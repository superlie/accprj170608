#ifndef DEBUG_MODE
    #define DEBUG_MODE
#endif

#ifndef AUTHORIZATION
    //#define AUTHORIZATION
#endif

#ifndef PRINT_REMOTE_DEBUG
    #define PRINT_REMOTE_DEBUG
#endif

//按下accept后本地校验数据的精度
#ifndef INACCURACY
    #define INACCURACY 1
#endif

//设置治疗数据时两个操作之间的延时
#ifndef INPUT_DELAY
    #define INPUT_DELAY //msleep(1000)
#endif

#ifndef HEARTBEAT_SEND_CYCLE
    #define HEARTBEAT_SEND_CYCLE 1000
#endif

#ifndef HEARTBEAT_RECV_CYCLE
    #define HEARTBEAT_RECV_CYCLE 4000
#endif

#ifndef CONNECT_TIMEOUT
    #define CONNECT_TIMEOUT 5000
#endif

//上位机和mlc移动到的控制点的误差
#ifndef BEAM_POSITION_DELTA
    #define BEAM_POSITION_DELTA 50
#endif

//出过线的dose值和总dose值之间的误差
#ifndef DELIVERED_DOSE_PRECISION
    #define DELIVERED_DOSE_PRECISION 0.8
#endif
