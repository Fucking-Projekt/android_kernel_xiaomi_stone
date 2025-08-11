#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/lz4.h>

#include "backend_lz4p.h"
#include "lz4p/lz4p.h"

struct lz4p_ctx {
	void *mem;
};

static void lz4p_destroy(struct zcomp_ctx *ctx)
{
	struct lz4p_ctx *zctx = ctx->context;

	if (!zctx)
		return;

	vfree(zctx->mem);
	kfree(zctx);
}

static int lz4p_create(struct zcomp_params *params, struct zcomp_ctx *ctx)
{
	struct lz4p_ctx *zctx;

	zctx = kzalloc(sizeof(*zctx), GFP_KERNEL);
	if (!zctx)
		return -ENOMEM;

	ctx->context = zctx;
	zctx->mem = vmalloc(LZ4_MEM_COMPRESS);
	if (!zctx->mem)
		goto error;

	return 0;

error:
	lz4p_destroy(ctx);
	return -ENOMEM;
}

static int lz4p_compress(struct zcomp_params *params, struct zcomp_ctx *ctx,
			 struct zcomp_req *req)
{
	struct lz4p_ctx *zctx = ctx->context;
	int ret;

	ret = LZ4P_compress_default(req->src, req->dst, req->src_len,
				    req->dst_len, zctx->mem);
	if (!ret)
		return -EINVAL;
	req->dst_len = ret;
	return 0;
}

static int lz4p_decompress(struct zcomp_params *params, struct zcomp_ctx *ctx,
			   struct zcomp_req *req)
{
	int ret;

	ret = LZ4P_decompress_safe(req->src, req->dst, req->src_len,
				   req->dst_len, NULL);
	if (ret < 0)
		return -EINVAL;
	return 0;
}

static int lz4p_setup_params(struct zcomp_params *params)
{
	return 0;
}

static void lz4p_release_params(struct zcomp_params *params)
{
}

const struct zcomp_ops backend_lz4p = {
	.compress	= lz4p_compress,
	.decompress	= lz4p_decompress,
	.create_ctx	= lz4p_create,
	.destroy_ctx	= lz4p_destroy,
	.setup_params	= lz4p_setup_params,
	.release_params	= lz4p_release_params,
	.name		= "lz4p",
};
