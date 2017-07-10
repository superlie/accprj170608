#ifndef LANGUAGE_H
#define LANGUAGE_H

#define CHINESE

#ifdef CHINESE

#define MSG_REMOTE_PROMPT   "请等待......"

#define F1_TERAT    "F1  治疗"
#define F1_X_FIX_L  "F1  6MV  X固定"
#define F1_X_ROT_L  "F1  6MV  X旋转"
#define F1_ENERGY   "F1  能量"
#define F1_LOW      "F1  低剂量率"
#define F1_SYNC     "F1  同步钨门"
#define F1_X_JAWS   "F1  X 钨门"
#define F1_X1       "F1  X1 钨门"

#define F2_X_FIX_H  "F2  10MV  X固定"
#define F2_X_ROT_H  "F2  10MV  X旋转"
#define F2_MU       "F2  剂量"
#define F2_HIGH     "F2  高剂量率"
#define F2_INDEP    "F2  独立钨门"
#define F2_Y_JAWS   "F2  Y  钨门"
#define F2_X2       "F2  X2  钨门"

#define F3_SERVICE  "F3  维修"
#define F3_E_FIX    "F3  电子线固定"
#define F3_GANTRY   "F3  机架"
#define F3_COLLI    "F3  准直器"
#define F3_Y1       "F3  Y1  钨门"

#define F4_DIAGS    "F4  诊断"
#define F4_ROT      "F4  旋转治疗"
#define F4_MU_MIN   "F4  剂量率"
#define F4_ARC      "F4  角度"
#define F4_ACC      "F4  附件"
#define F4_Y2       "F4  Y2  钨门"

#define F5_RCL_DAT  "F5  回看"
#define F5_TIME     "F5  时间"

#define F6_SHUTDOWN "F6  关机"
#define F6_SETUP    "F6  设置"
#define F6_PRV_MNU  "F6  返回"

#define MON_1_UI       "剂量监视 1"
#define MON_2_UI       "剂量监视 2"
#define TIME_UI        "治疗时间"
#define MU_MIN_UI      "剂量率"
#define ENERGY_UI      "能量"
#define MODE_UI        "射线类型"
#define SEG_UI         "SEG"
#define TECHNIQUE_UI   "治疗方式"
#define COLLIMATOR_UI  "准直器"
#define G_SPEED_UI     "机架转速"
#define G_DIREC_UI     "旋转方向"
#define ACC1_UI        "附件  #1"
#define JAWS_UI        "钨门"
#define TRIMMERS_UI       "照射范围"
#define G_POS_UI       "机架位置"
#define ARC_UI         "弧度"
#define STOP_UI        "停止位置"
#define ACC_NONE_UI    "无"
#define ACC_INVALID_UI "未识别"

#define SERVICE_UI      "维修"

#define STANDBY_UI      "预  备"
#define PROGRAM_UI      "编  程"
#define READY_UI        "准备好"
#define NOT_READY_UI    "未准备好"
//#define RADON_UI        ""
#define INTERRUPT_UI    "中  断"
#define INTERRUPT_NR_UI "中断未准备好"
#define TERMINATE_UI    "终  止"
#define COMPLETE_UI     "完  成"
#define SETUP_UI        "设  置"
#define ERROR_UI        "错  误"

#define X_RAY_UI        "X射线"
#define ELTRN_UI        "电子线"
#define FIX_UI          "固定"
#define ROT_UI          "旋转"


#define MSG_WAIT            "请等待......"
#define MSG_LKEY_RESET      "将左手钥匙开关旋转到“RESET”"
#define MSG_LKEY_TREAT      "将左手钥匙开关旋转到“TREAT”"
#define MSG_LKEY_SVC        "将左手钥匙开关旋转到“SERVICE”"
#define MSG_RKEY_PGM        "将右手钥匙开关旋转到“PGM/RDY”"
#define MSG_RKEY_RADON      "将右手钥匙开关旋转到“RAD ON”"
#define MSG_RKEY_STDBY_PGM  "将右手钥匙开关旋转到“STD BY”,然后旋转回“PGM/RDY”."

#define MSG_SVC_PASSWORD    "请输入密码，按<ENTER>键确认"
#define MSG_REENTER_PASSWORD    "密码错误，请重新输入："

#define MSG_ENTER_DOSE1     "请输入剂量监视1的值，按<ENTER>键确认"
#define MSG_SURE_DOSE1      "是否认可(Y/N)?"
#define MSG_REENTER_DOSE1   "请再次输入，确认剂量监视单位值"
#define MSG_ENTER_TIME      "请输入治疗时间，按<ENTER>键确认"

#define MSG_ENTER_JAWX      "请输入钨门X的值，按<ENTER>键确认"
#define MSG_ENTER_JAWX1     "请输入钨门X1的值，按<ENTER>键确认"
#define MSG_ENTER_JAWX2     "请输入钨门X2的值，按<ENTER>键确认"
#define MSG_ENTER_JAWY      "请输入钨门Y的值，按<ENTER>键确认"
#define MSG_ENTER_JAWY1     "请输入钨门Y1的值，按<ENTER>键确认"
#define MSG_ENTER_JAWY2     "请输入钨门Y2的值，按<ENTER>键确认"
#define MSG_ENTER_COLLI     "请输入准直器的值，按<ENTER>键确认"
#define MSG_ENTER_GPOS      "请输入机架的旋转位置，按<ENTER>键确认"
#define MSG_ENTER_ARC       "请输入机架的旋转角度，按<ENTER>键确认"
#define MSG_LIMIT_REENTER   "输入的值超过范围，请重新输入"
#define MSG_CTRL_X          "错误---按X键重新输入"
#define MSG_GPOS_NOTMATCH   "实际数据与预置值不匹配"
#define MSG_PARA_COMPLETE   "请输入完整的治疗数据"

#define MSG_CLEAR_QUIT      "请清除联锁后继续，或者按<F1 QUIT>键终止"
#define MSG_RADON_QUIT      "请按<RAD ON>键继续，或者按<F1 QUIT>键终止"
#define MSG_CLEAR_KEY       "请按<CLEAR>键"
#define MSG_SOFTPOT_COPY_DEV "RESEARCH data deviation"

#define MSG_REMOTE_CANCEL   "处于远程模式，按下CLEAR退出"
#define MSG_REMOTE_TERMINATE   "处于远程模式，按下CLEAR清除数据"

#else //English

#define F1_TERAT    "F1  TREAT"
#define F1_X_FIX_L  "F1  X-FIX-L"
#define F1_X_ROT_L  "F1  X-ROT-L"
#define F1_ENERGY   "F1  ENERGY"
#define F1_LOW      "F1  LOW"
#define F1_SYNC     "F1  SYNC"
#define F1_X_JAWS   "F1  X JAWS"
#define F1_X1       "F1  X1"

#define F2_X_FIX_H  "F2  X-FIX-H"
#define F2_X_ROT_H  "F2  X-ROT-H"
#define F2_MU       "F2  MU"
#define F2_HIGH     "F2  HIGH"
#define F2_INDEP    "F2  INDEP"
#define F2_Y_JAWS   "F2  Y JAWS"
#define F2_X2       "F2  X2"

#define F3_SERVICE  "F3  SERVICE"
#define F3_E_FIX    "F3  E-FIX"
#define F3_GANTRY   "F3  GANTRY"
#define F3_COLLI    "F3  COLLI"
#define F3_Y1       "F3  Y1"

#define F4_DIAGS    "F4  DIAGS"
#define F4_ROT      "F4  ROT"
#define F4_MU_MIN   "F4  MU/MIN"
#define F4_ARC      "F4  ARC"
#define F4_ACC      "F4  ACC"
#define F4_Y2       "F4  Y2"

#define F5_RCL_DAT  "F5  RCL DAT"
#define F5_TIME     "F5  TIME"

#define F6_SHUTDOWN "F6  SHUTDOWN"
#define F6_SETUP    "F6  SETUP"
#define F6_PRV_MNU  "F6  PRV MNU"

#define MON_1_UI       "MON 1"
#define MON_2_UI       "MON 2"
#define TIME_UI        "TIME"
#define MU_MIN_UI      "MU/MIN"
#define ENERGY_UI      "ENERGY"
#define MODE_UI        "MODE"
#define TECHNIQUE_UI   "TECHNIQUE"
#define COLLIMATOR_UI  "COLLIMATOR"
#define G_SPEED_UI     "G-SPEED"
#define G_DIREC_UI     "G-DIREC"
#define ACC1_UI        "ACC  #1"
#define JAWS_UI        "JAWS"
#define G_POS_UI       "G-POS"
#define ARC_UI         "ARC"
#define STOP_UI        "STOP"
#define ACC_NONE_UI    "NONE"
#define ACC_INVALID_UI "INVALID"

#define SERVICE_UI      "SVC"

#define STANDBY_UI      "STAND BY"
#define PROGRAM_UI      "PROGRAM"
#define READY_UI        "READY"
#define NOT_READY_UI    "NOT READY"
//#define RADON_UI        ""
#define INTERRUPT_UI    "INTERRUPT"
#define INTERRUPT_NR_UI "INTERRUPT NR"
#define TERMINATE_UI    "TERMINATE"
#define COMPLETE_UI     "COMPLETE"
#define SETUP_UI        "SET UP"
#define ERROR_UI        "ERROR"

#define X_RAY_UI        "X-RAY"
#define ELTRN_UI        "ELTRN"
#define FIX_UI          "FIXED"
#define ROT_UI          "ROT"



#define MSG_WAIT            "Please waiting ......"
#define MSG_LKEY_RESET      "Please turn left key switch to RESET."
#define MSG_LKEY_TREAT      "Please turn left key switch to TREAT."
#define MSG_LKEY_SVC        "Please turn left key switch to SERVCE."
#define MSG_RKEY_PGM        "Please turn right key switch to PGM/RDY."
#define MSG_RKEY_RADON      "Please turn right key switch to RAD ON"
#define MSG_RKEY_STDBY_PGM  "Please turn right key switch to STD BY then to PGM/RDY"

#define MSG_SVC_PASSWORD    "请输入密码："
#define MSG_REENTER_PASSWORD    "密码错误，请重新输入："

#define MSG_ENTER_DOSE1     "Enter the dose value, then press <RETURN>or<Enter>"
#define MSG_SURE_DOSE1      "是否认可(Y/N)?"
#define MSG_REENTER_DOSE1   "请确认剂量监视单位值:"
#define MSG_ENTER_TIME      "Enter the time, then press <Enter>"
#define MSG_PARA_COMPLETE   ""

#define MSG_ENTER_JAWX      "Enter jaws size X, then press <Enter>"
#define MSG_ENTER_JAWX1     "Enter 'X1' value, then press <RETURN> or <Enter>"
#define MSG_ENTER_JAWX2     "Enter 'X2' value, then press <RETURN> or <Enter>"
#define MSG_ENTER_JAWY      "Enter jaws size Y, then press <Enter>"
#define MSG_ENTER_JAWY1     "Enter 'Y1' value, then press <RETURN> or <Enter>"
#define MSG_ENTER_JAWY2     "Enter 'Y2' value, then press <RETURN> or <Enter>"
#define MSG_ENTER_COLLI     "Enter the collimator position, then press <Enter>"
#define MSG_ENTER_GPOS      "Enter the gantry position, then press <Enter>"
#define MSG_ENTER_ARC       "Enter arc #1 degrees, then press <Enter>"
#define MSG_LIMIT_REENTER   "Error - Value(s) out of limit, please re-enter"
#define MSG_CTRL_X          "错误---按ctrl x重新输入"
#define MSG_GPOS_NOTMATCH   "Accept key ignored until actual data matches preset data"

#define MSG_CLEAR_QUIT      "Clear interlocks or press F1 to terminate ..."
#define MSG_RADON_QUIT      "Press RAD ON key to continue, F1 to terminate ..."
#define MSG_CLEAR_KEY       "Please press CLEAR key"

#define MSG_REMOTE_CANCEL   "KERBOARD IN REMOTE MODE-CLEAR TO CANCEL"
#define MSG_REMOTE_TERMINATE   "KERBOARD IN REMOTE MODE-CLEAR TO TERMINATE"


#endif//English

#endif // LANGUAGE_H
