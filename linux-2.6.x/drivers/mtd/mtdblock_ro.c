/*
 * $Id: mtdblock_ro.c 903 2006-04-06 01:12:19Z bthaeler $
 *
 * (C) 2003 David Woodhouse <dwmw2@infradead.org>
 *
 * Simple read-only (writable only for RAM) mtdblock driver
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/blktrans.h>

static int mtdblock_readsect(struct mtd_blktrans_dev *dev,
			      unsigned long block, char *buf)
{
	size_t retlen;

	if (dev->mtd->read(dev->mtd, (block * 512), 512, &retlen, buf))
		return 1;
	return 0;
}

static int mtdblock_writesect(struct mtd_blktrans_dev *dev,
			      unsigned long block, char *buf)
{
	size_t retlen;

	if (dev->mtd->write(dev->mtd, (block * 512), 512, &retlen, buf))
		return 1;
	return 0;
}

static void mtdblock_add_mtd(struct mtd_blktrans_ops *tr, struct mtd_info *mtd)
{
	struct mtd_blktrans_dev *dev = kmalloc(sizeof(*dev), GFP_KERNEL);

	if (!dev)
		return;

	memset(dev, 0, sizeof(*dev));

	dev->mtd = mtd;
	dev->devnum = mtd->index;
	dev->blksize = 512;
	dev->size = mtd->size >> 9;
	dev->tr = tr;
	if ((mtd->flags & (MTD_CLEAR_BITS|MTD_SET_BITS|MTD_WRITEABLE)) !=
	    (MTD_CLEAR_BITS|MTD_SET_BITS|MTD_WRITEABLE))
		dev->readonly = 1;

	add_mtd_blktrans_dev(dev);
}

static void mtdblock_remove_dev(struct mtd_blktrans_dev *dev)
{
	del_mtd_blktrans_dev(dev);
	kfree(dev);
}

static int 
mtdblock_direct(struct mtd_blktrans_dev *dev, sector_t sector, unsigned long *data)
{
	struct mtd_info *mtd = dev->mtd;
	size_t len;

	if (!mtd->point)
		return -ENOSYS;
	return (mtd->point)(mtd,sector * 512,0,&len,(u_char **) data);
}

static struct mtd_blktrans_ops mtdblock_tr = {
	.name		= "mtdblock",
	.major		= 31,
	.part_bits	= 0,
	.readsect	= mtdblock_readsect,
	.writesect	= mtdblock_writesect,
	.add_mtd	= mtdblock_add_mtd,
	.remove_dev	= mtdblock_remove_dev,
	.owner		= THIS_MODULE,
	.direct_access  = mtdblock_direct,
};

static int __init mtdblock_init(void)
{
	return register_mtd_blktrans(&mtdblock_tr);
}

static void __exit mtdblock_exit(void)
{
	deregister_mtd_blktrans(&mtdblock_tr);
}

module_init(mtdblock_init);
module_exit(mtdblock_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Woodhouse <dwmw2@infradead.org>");
MODULE_DESCRIPTION("Simple read-only block device emulation access to MTD devices");
