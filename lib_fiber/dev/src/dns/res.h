#ifndef __RES_INCLUDE_H__
#define __RES_INCLUDE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

/**
 * DNS���ؽ���Ĵ洢�ṹ
 */
typedef struct RES {
	char dns_ip[64];                /**< DNS��IP��ַ */
	unsigned short dns_port;        /**< DNS��Port */
	unsigned short cur_qid;         /**< �ڲ����������ݰ��ı�ʶ */
	time_t tm_spent;                /**< ��ѯʱ��ķ�(��) */
	int   errnum;
#define RES_ERR_SEND	-100    /**< д���� */
#define RES_ERR_READ	-101    /**< ������ */
#define RES_ERR_RTMO	-102    /**< ����ʱ */
#define RES_ERR_NULL	-103    /**< �ս�� */
#define RES_ERR_CONN	-104    /**< TCP��ʽʱ����ʧ�� */

	int transfer;           /**< TCP/UDP ����ģʽ */
#define RES_USE_UDP	0       /**< UDP ����ģʽ */
#define RES_USE_TCP	1       /**< TCP ����ģʽ */

	int   conn_timeout;     /**< TCP ����ʱ�����ӳ�ʱʱ��, Ĭ��Ϊ10�� */
	int   rw_timeout;       /**< TCP/UDP �����IO��ʱʱ��, Ĭ��Ϊ10�� */
} RES;

/**
 * ����һ��DNS��ѯ����
 * @param dns_ip {const char*} DNS��IP��ַ
 * @param dns_port {unsigned short} DNS��Port
 * @return {RES*} �´����Ĳ�ѯ����
 */
RES *res_new(const char *dns_ip, unsigned short dns_port);

/**
 * ����DNS��ѯ�ĳ�ʱʱ��
 * @param conn_timeout {int} TCP ����ʱ�����ӳ�ʱʱ��
 * @param rw_timeout {int} TCP/UDP �����IO��ʱʱ��
 */
void res_set_timeout(int conn_timeout, int rw_timeout);

/**
 * �ͷ�һ��DNS��ѯ����
 * @param res {RES*} DNS��ѯ����
 */
void res_free(RES *res);

/**
 * ��ѯĳ��������IP��ַ
 * @param res {RES*} DNS��ѯ����
 * @param domain {const char*} Ҫ��ѯ������
 * @return {DNS_DB*} ��ѯ�Ľ����
 */
DNS_DB *res_lookup(RES *res, const char *domain);

/**
 * ���ݴ���Ż�ò�ѯʧ�ܵ�ԭ��
 * @param errnum {int} �����
 * @return {const char*} ������Ϣ
 */
const char *res_strerror(int errnum);

/**
 * ��õ�ǰ��ѯ�Ĵ�����Ϣ
 * @param res {RES*} DNS��ѯ����
 * @return {const char*} ������Ϣ
 */
const char *res_errmsg(const RES *res);

#ifdef __cplusplus
}
#endif

#endif

