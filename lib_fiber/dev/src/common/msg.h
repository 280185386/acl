#ifndef _MSG_INCLUDE_H_
#define _MSG_INCLUDE_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdio.h>
#include "define.h"

#undef	USE_PRINTF_MACRO

/**
 * �ڽ�д��־����־�ļ�ǰ�ص��û��Զ���ĺ������ҽ���־��Ϣ���ݸ��ú�����
 * ֻ�е��û�ͨ�� msg_pre_write �������ú����Ч
 * @param ctx {void*} �û����Զ������
 * @param fmt {const char*} ��ʽ����
 * @param ap {va_list} ��ʽ�����б�
 */
typedef void (*MSG_PRE_WRITE_FN)(void *ctx, const char *fmt, va_list ap);

/**
 * Ӧ��ͨ���˺������Ϳ����Զ�����־�򿪺�������Ӧ���ڴ���־ǰ����
 * msg_register ע�����Զ���򿪺�������Ӧ�õ��� msg_open
 * ʱ����ô˶������־��������־����������ȱʡ�ķ�������־�ļ�
 * @param file_name {const char*} �ش����Զ�����־�򿪺����Ĳ�������
 *  ����־�ļ��ش�
 * @param ctx {void*} Ӧ�ô��ݽ�ȥ�Ĳ���
 * @return {int} ����Զ������־�������� -1 �����ȱʡ����־�򿪺���
 */
typedef int (*MSG_OPEN_FN) (const char *file_name, void *ctx);

/**
 * Ӧ��ͨ���˺������Ϳ����Զ�����־�رպ�������Ӧ���ڴ���־ǰ����
 * msg_register ע�����Զ���򿪺���
 * @param ctx {void*} Ӧ�ô��ݽ�ȥ�Ĳ���
 */
typedef void (*MSG_CLOSE_FN) (void *ctx);

/**
 * Ӧ��ͨ���˺������Ϳ����Զ�����־��¼��������Ӧ���ڴ���־ǰ����
 * msg_register ע�����Զ����¼��������Ӧ��д��־ʱ���ô��Զ���
 * ������¼��־��������ȱʡ����־��¼����
 * @param ctx {void*} Ӧ�ô��ݽ�ȥ�Ĳ���
 * @param fmt {const char*} ��ʽ����
 * @param ap {va_list} �����б�
 */
typedef void (*MSG_WRITE_FN) (void *ctx, const char *fmt, va_list ap);

/**
 * �ڴ���־ǰ���ô˺���ע��Ӧ���Լ�����־�򿪺�������־�رպ�������־��¼����
 * @param open_fn {MSG_OPEN_FN} �Զ�����־�򿪺���
 * @param close_fn {MSG_CLOSE_FN} �Զ�����־�رպ���
 * @param write_fn {MSG_WRITE_FN} �Զ�����־��¼����
 * @param ctx {void*} �Զ������
 */
void msg_register(MSG_OPEN_FN open_fn, MSG_CLOSE_FN close_fn,
        MSG_WRITE_FN write_fn, void *ctx);

/**
 * �� msg_register ע���Զ��庯�����������ȱʡ����־������
 */
void msg_unregister(void);

/**
 * �ڴ���־ǰ���ô˺���ע��Ӧ�õ�˽�к������ڼ�¼��־ǰ���ȼ�¼��Ϣͨ��
 * ��ע��ĺ������ݸ�Ӧ��
 * @param pre_write {MSG_PRE_WRITE_FN} ��־��¼ǰ���õĺ���
 * @param ctx {void*} �Զ������
 */
void msg_pre_write(MSG_PRE_WRITE_FN pre_write, void *ctx);

/**
 * ��δ���� msg_open ��ʽ����־ʱ�������� msg_info/error/fatal/warn
 * �Ĳ������Ƿ�������Ϣ�������׼�����Ļ�ϣ�ͨ���˺��������øÿ��أ��ÿ���
 * ��Ӱ���Ƿ���Ҫ����Ϣ������ն���Ļ����Ӱ���Ƿ�������ļ���
 * @param onoff {int} �� 0 ��ʾ�����������Ļ
 */
void msg_stdout_enable(int onoff);

/**
 * ����¼��־��Ϣ����־�ļ�ʱ����Ҫ�������µ���־��¼����
 */

#ifndef	USE_PRINTF_MACRO

/**
 * һ�㼶����־��Ϣ��¼����
 * @param fmt {const char*} ������ʽ
 * @param ... �������
 */
void PRINTF(1, 2) msg_info(const char *fmt,...);

/**
 * ���漶����־��Ϣ��¼����
 * @param fmt {const char*} ������ʽ
 * @param ... �������
 */
void PRINTF(1, 2) msg_warn(const char *fmt,...);

/**
 * ���󼶱���־��Ϣ��¼����
 * @param fmt {const char*} ������ʽ
 * @param ... �������
 */
void PRINTF(1, 2) msg_error(const char *fmt,...);

/**
 * ����������־��Ϣ��¼����
 * @param fmt {const char*} ������ʽ
 * @param ... �������
 */
void PRINTF(1, 2) msg_fatal(const char *fmt,...);

/**
 * һ�㼶����־��Ϣ��¼����
 * @param fmt {const char*} ������ʽ
 * @param ap {va_list} ����б�
 */
void msg_info2(const char *fmt, va_list ap);

/**
 * ���漶����־��Ϣ��¼����
 * @param fmt {const char*} ������ʽ
 * @param ap {va_list} ����б�
 */
void msg_warn2(const char *fmt, va_list ap);

/**
 * ���󼶱���־��Ϣ��¼����
 * @param fmt {const char*} ������ʽ
 * @param ap {va_list} ����б�
 */
void msg_error2(const char *fmt, va_list ap);


/**
 * ����������־��Ϣ��¼����
 * @param fmt {const char*} ������ʽ
 * @param ap {va_list} ����б�
 */
void msg_fatal2(const char *fmt, va_list ap);

#else

/**
 * ����¼��־��Ϣ����׼���ʱ����Ҫ�������µ���־��¼����
 */

#include <stdio.h>

#undef	msg_info
#undef	msg_warn
#undef	msg_error
#undef	msg_fatal
#undef	msg_panic

#define	msg_info	msg_printf
#define	msg_warn	msg_printf
#define	msg_error	msg_printf
#define	msg_fatal	msg_printf
#define	msg_panic	msg_printf

#endif

const char *msg_strerror(int errnum, char *buffer, size_t size);

/**
 * ����ϴ�ϵͳ���ó���ʱ�Ĵ���������Ϣ
 * @param buffer {char*} �洢����������Ϣ���ڴ滺����
 * @param size {size_t} buffer �Ŀռ��С
 * @return {const char*} ���صĵ�ַӦ�� buffer ��ͬ
 */
const char *last_strerror(char *buffer, size_t size);

/**
 * ����ϴ�ϵͳ���ó���ʱ�Ĵ���������Ϣ���ú����ڲ��������ֲ߳̾�������������
 * �̰߳�ȫ�ģ���ʹ����������Щ
 * @return {const char *} ���ش�����ʾ��Ϣ 
 */
const char *last_serror(void);

/**
 * ����ϴ�ϵͳ���ó���ʱ�Ĵ����
 * @return {int} �����
 */
int last_error(void);

/**
 * �ֹ����ô����
 * @param errnum {int} �����
 */
void set_error(int errnum);

/**
 * �����Ϣ����׼���
 * @param fmt {const char*} ��ʽ����
 * @param ... �������
 */
void PRINTF(1, 2) msg_printf(const char *fmt,...);

#ifdef  __cplusplus
}
#endif

#endif
