#ifndef	__NETDB_INCLUDE_H__
#define	__NETDB_INCLUDE_H__

/**
 * ������ַ�ṹ
 */
typedef struct HOSTNAME HOST_INFO;
typedef struct HOSTNAME {
	char  ip[64];                   /**< the ip addr of the HOST */
	struct sockaddr_in saddr;	/**< ip addr in sockaddr_in */
	unsigned int ttl;               /**< the HOST's ip timeout(second) */
	int   hport;
	unsigned int nrefer;            /**< refer number to this HOST */
} HOSTNAME;

/**
 * DNS��ѯ�����
 */
typedef struct DNS_DB {
	ARRAY *h_db;
	int   size;
	char  name[256];

	/* for iterator */

	/* ȡ������ͷ���� */
	const HOST_INFO *(*iter_head)(ITER*, struct DNS_DB*);
	/* ȡ��������һ������ */
	const HOST_INFO *(*iter_next)(ITER*, struct DNS_DB*);
	/* ȡ������β���� */
	const HOST_INFO *(*iter_tail)(ITER*, struct DNS_DB*);
	/* ȡ��������һ������ */
	const HOST_INFO *(*iter_prev)(ITER*, struct DNS_DB*);
	/* ȡ�����������ĵ�ǰ������Ա�ṹ���� */
	const HOST_INFO *(*iter_info)(ITER*, struct DNS_DB*);
} DNS_DB;

/**
 *  �ͷŽ�����ڴ���Դ
 * @param h_dns_db {DNS_DB*} DNS�����
 */
void netdb_free(DNS_DB *h_dns_db);

/**
 * ������������һ����ѯ������Ľṹ������������DNS��ѯ
 * @param domain {const char*} Ҫ��ѯ������
 * @return {DNS_DB*} �����Ľ��������
 */
DNS_DB *netdb_new(const char *domain);

#endif
