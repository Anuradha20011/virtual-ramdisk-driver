#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include <linux/vmalloc.h>
#include <crypto/skcipher.h>
#include <linux/scatterlist.h>
#include <linux/crypto.h>
#include <linux/random.h>
#include <linux/unaligned.h>
#include <linux/completion.h>
#include <linux/slab.h>

#define DEVICE_NAME "virtual_ramdisk"
#define RAMDISK_SIZE (16 * 1024 * 1024)
#define VRD_BLOCK_SIZE 512

static unsigned char *ramdisk_data;

static struct blk_mq_tag_set tag_set;
static struct request_queue *ramdisk_queue;
static struct gendisk *ramdisk_disk;
static struct crypto_skcipher *xts_tfm;
static struct skcipher_request *xts_req;
static unsigned char xts_key[64];
static unsigned char xts_iv[16];
static unsigned char *crypto_buf;
static void virtual_ramdisk_make_iv(sector_t sector)
{
	memset(xts_iv, 0, sizeof(xts_iv));    	put_unaligned_le64(sector, xts_iv);
}

static int virtual_ramdisk_crypt_sector(void *buf, sector_t sector, bool encrypt)
{
    	struct scatterlist sg;
    	DECLARE_CRYPTO_WAIT(wait);
    	int ret;

    	virtual_ramdisk_make_iv(sector);

    	sg_init_one(&sg, buf, VRD_BLOCK_SIZE);

    	skcipher_request_set_callback(xts_req, CRYPTO_TFM_REQ_MAY_BACKLOG | CRYPTO_TFM_REQ_MAY_SLEEP,crypto_req_done, &wait);

    	skcipher_request_set_crypt(xts_req, &sg, &sg, VRD_BLOCK_SIZE, xts_iv);

    	if (encrypt) {
        	ret = crypto_wait_req(crypto_skcipher_encrypt(xts_req), &wait);
    	}
	else {
       		ret = crypto_wait_req(crypto_skcipher_decrypt(xts_req), &wait);
    	}

    	return ret;
}

		static blk_status_t virtual_ramdisk_queue_rq(
    struct blk_mq_hw_ctx *hctx,
    const struct blk_mq_queue_data *bd)
{
    struct request *req = bd->rq;
    struct bio_vec bvec;
    struct req_iterator iter;
    sector_t sector = blk_rq_pos(req);

    blk_mq_start_request(req);

    rq_for_each_segment(bvec, req, iter) {

        unsigned int len = bvec.bv_len;
        unsigned int offset = sector * VRD_BLOCK_SIZE;
        unsigned int processed = 0;

        if (offset + len > RAMDISK_SIZE) {
            blk_mq_end_request(req, BLK_STS_IOERR);
            return BLK_STS_OK;
        }

        while (processed < len) {

            	struct bio_vec sector_bvec = bvec;
            	sector_t current_sector = sector + processed / VRD_BLOCK_SIZE;
            	unsigned int current_offset = offset + processed;

            	sector_bvec.bv_offset += processed;
            	sector_bvec.bv_len = VRD_BLOCK_SIZE;

            	if (rq_data_dir(req) == WRITE) {

                	memcpy_from_bvec(crypto_buf, &sector_bvec);

			if (virtual_ramdisk_crypt_sector(crypto_buf, current_sector, true) != 0) {
        			blk_mq_end_request(req, BLK_STS_IOERR);
        			return BLK_STS_OK;
    			}

                	memcpy(ramdisk_data + current_offset, crypto_buf, VRD_BLOCK_SIZE);

            	}
            	else {
                	memcpy(crypto_buf, ramdisk_data + current_offset, VRD_BLOCK_SIZE);

                	if (virtual_ramdisk_crypt_sector(crypto_buf, current_sector, false) != 0) {

                    		blk_mq_end_request(req, BLK_STS_IOERR);
                    		return BLK_STS_OK;
                	}

                	memcpy_to_bvec(&sector_bvec, crypto_buf);
            		}

            		processed += VRD_BLOCK_SIZE;
        	}

		sector += len / VRD_BLOCK_SIZE;
	}

	blk_mq_end_request(req, BLK_STS_OK);
	return BLK_STS_OK;
}

static const struct block_device_operations virtual_ramdisk_fops = {
    	.owner = THIS_MODULE,
};

static const struct blk_mq_ops virtual_ramdisk_mq_ops = {.queue_rq = virtual_ramdisk_queue_rq,};

static int __init virtual_ramdisk_init(void){
	struct queue_limits lim = {.logical_block_size = VRD_BLOCK_SIZE, .physical_block_size = VRD_BLOCK_SIZE
};
	int ret;

	ramdisk_data = vmalloc(RAMDISK_SIZE);
	if (!ramdisk_data)
		return -ENOMEM;

	memset(ramdisk_data, 0, RAMDISK_SIZE);

	xts_tfm = crypto_alloc_skcipher("xts(aes)", 0, 0);
	if (IS_ERR(xts_tfm)) {
    		ret = PTR_ERR(xts_tfm);
    		vfree(ramdisk_data);
    		return ret;
	}

	get_random_bytes(xts_key, sizeof(xts_key));

	ret = crypto_skcipher_setkey(xts_tfm, xts_key, sizeof(xts_key));
	if (ret) {
    		crypto_free_skcipher(xts_tfm);
    		vfree(ramdisk_data);
    		return ret;
	}

	xts_req = skcipher_request_alloc(xts_tfm, GFP_KERNEL);
	if (!xts_req) {
    		crypto_free_skcipher(xts_tfm);
    		vfree(ramdisk_data);
    		return -ENOMEM;
	}

	crypto_buf = kmalloc(VRD_BLOCK_SIZE, GFP_KERNEL);
	if (!crypto_buf) {
    		skcipher_request_free(xts_req);
    		crypto_free_skcipher(xts_tfm);
    		vfree(ramdisk_data);
    		return -ENOMEM;
	}

	memset(&tag_set, 0, sizeof(tag_set));

	tag_set.ops = &virtual_ramdisk_mq_ops;
	tag_set.nr_hw_queues= 1;
	tag_set.nr_maps = 1;
	tag_set.queue_depth = 1;
	tag_set.numa_node = NUMA_NO_NODE;

	ret =blk_mq_alloc_tag_set(&tag_set);
	if (ret) {
		vfree(ramdisk_data);
		return ret;
	}

	ramdisk_disk = blk_mq_alloc_disk(&tag_set, &lim, NULL);
	if (IS_ERR(ramdisk_disk)) {
		ret = PTR_ERR(ramdisk_disk);
		blk_mq_free_tag_set(&tag_set);
		vfree(ramdisk_data);
		return ret;
	}

	ramdisk_disk->fops = &virtual_ramdisk_fops;

	strscpy(ramdisk_disk->disk_name, DEVICE_NAME, DISK_NAME_LEN);

	set_capacity(ramdisk_disk, RAMDISK_SIZE / VRD_BLOCK_SIZE);

	ret = device_add_disk(NULL, ramdisk_disk, NULL);
	if (ret) {
    		put_disk(ramdisk_disk);
    		blk_mq_free_tag_set(&tag_set);
    		vfree(ramdisk_data);
    		return ret;
	}

	ramdisk_queue = ramdisk_disk->queue;

	return 0;
}

static void __exit virtual_ramdisk_exit(void)
{
    	del_gendisk(ramdisk_disk);
    	put_disk(ramdisk_disk);

    	blk_mq_free_tag_set(&tag_set);

    	kfree(crypto_buf);
    	skcipher_request_free(xts_req);
    	crypto_free_skcipher(xts_tfm);

    	vfree(ramdisk_data);
}
module_exit(virtual_ramdisk_exit);
module_init(virtual_ramdisk_init);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("In-memory virtual RAM disk block driver");
