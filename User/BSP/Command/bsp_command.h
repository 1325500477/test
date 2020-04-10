//Tab size: 4;	Indent size: 4
#ifndef __BSP_COMMAND_H__
#define __BSP_COMMAND_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*
20160522:
    ����������۾�����IAR��226R4+227R4��BSPϵͳ���е���������ɸѡ����Ԥ�������£�
    1)1~99������Ϊ������������
    2)100������Ϊָ��ĳ����ŵĵ�������ָ���ʽΪ��100������+ָ�����������+�ö��������������
    3)101~199������Ϊ�û����ɶ��������UserCode��Command��command.c��ProcessSUserCmd���н���
    4)200~255������Ϊϵͳ���������BSP��Command��bsp_command.c��ProcessSystemCmd���н���
*/
//������ϵͳ�������������������ͬ��ά����
#define CMD_PROGRAM         		200			// ����Bootloader���³���
#define CMD_SET_DC_MOTOR			201			// ����ֱ��������
#define CMD_SET_STEP_MOTOR			202			// ���ò���������
#define CMD_GET_FIRMWARE		    203			// ��ȡAPP�̼���Ϣ������BSP�̼��汾��Ϣ��
#define CMD_GET_ALL_INFO			204			// --���ɲ��ã���ȡ����IO״̬�����롢����Լ����������
#define CMD_GET_SENSOR				205			// ��ȡ�������ź�
#define CMD_GET_PARAMETER			206			// ��ȡ�豸��������
#define CMD_SET_PARAMETER			207			// �����豸��������
#define CMD_GET_DEFAULT_VALUE		208			// �ָ�����ֵ
#define CMD_SET_DEFAULT_VALUE		209			// ���óɳ���ֵ

#define CMD_GET_OP_INFO 		    210			// ��ȡĳ��ָ�������̶��������ʱ�䣬״̬�������Ϣ
#define CMD_GET_ALL_OP_RESULT       211         // --���ɲ��ã���ȡ���̽���������ϴ����ж������������������Ҫ���ٵĳ��ϣ�
#define CMD_GET_ALL_OP_TIME         212         // --���ɲ��ã���ȡ����ʱ�䣨�����ϴ����ж���ʱ�䣩
#define CMD_RESUME_OP				213			// �ָ�����(�豸������˹��ָ���������)
#define CMD_START_OP_BY_MODE        214			// �������̴�START����ʼ���У����в�����0Ϊֱ�����У�1Ϊ�����ķ�ʽ����)
#define CMD_DO_OP_NEXT_STEP			215			// ִ��������һ��
#define CMD_SET_OP_MODE             216         // ��������ģʽ

#define CMD_SUB_NET					254			// �ڲ����ư�ͨѶָ���һ���ֽ�ΪM��ʾ������ΪS��ʾ�ӻ���
#define CMD_HAND_SHAKE				255			// ����ָ��

//STM32ר��
#define CMD_UPDATE_FLASH            197
#define CMD_JUMP_TO_APP             198
#define CMD_JUMP_TO_BOOTLOADER      199

void JumpToApp(void);
void JumpToBootload(void);

extern void ProcessSystemCmd(void);

#ifdef __cplusplus
}
#endif

#endif
