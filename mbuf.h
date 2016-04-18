#ifndef _MBUF_H_
#define _MBUF_H_

#include <stdlib.h>

#include "vhost.h"

#define MBUF_DATA_LEN	(2048)

struct mbuf {
	u32 len;
	char *data;
	struct virtio_net_hdr hdr;
};

// y is 2^n
#define ROUNDDN(x, y)	(x & (~(y - 1)))

static inline void vhost_dump_mbuf(struct mbuf *m)
{
	int i = 0;

	vhost_log("mbuf: len %u\n", m->len);	
	for (i = 0; i < ROUNDDN(m->len, 4); i++) {
		vhost_log("%02x %02x %02x %02x\n",
				m->data[i], m->data[i+1], m->data[i+2], m->data[i+3]);
	}
	for (; i < m->len; i++) {
		vhost_log("%02x ", m->data[i]);
	}
	vhost_log("\n");
}

static inline struct mbuf *vhost_new_mbuf(void)
{
	struct mbuf *m;

	m = (struct mbuf*)malloc(sizeof(*m));
	if (!m)
		return 0;
	m->len = 0;
	m->data = malloc(MBUF_DATA_LEN);
	if (!m->data) {
		free(m);
		return 0;
	}
}

static inline void vhost_free_mbuf(struct mbuf *m)
{
	if (m->data)
		free(m->data);
	m->data = 0;
	m->len = 0;
	free(m);
}

#endif