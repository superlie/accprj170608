#include "beam.h"
#include "ui/accui.h"

extern AccUi *accui;

Beam::Beam()
{
    slectmode = RESET_MODE;
    check = CHECK1;
    state = NOTREADY;

    afcm = 0;
    beami = 0;
    injpi = 0;
    phsm = 0;

    elmi = 0;   //elm
    bmis = 0;      //bmi
    stci = 0;     //stc
    leni = 0;      //len
    pfne = 0;
    error_now = 0;
    line_status = 0;
    check_report = 0;
    prom_version = 0;

    send_frame_num = 0;
    rev_frame_num = 0;

    for(int i=0; i<51; i++)
        item_has[i] = false;
}

Beam::~Beam()
{
    ;
}

//读取射线文件中的参数并且赋值
bool Beam::ReadElement()
{
    //qDebug()<<"Read Beam element";
    for(int i=0; i<51; i++)
        item_has[i] = false;

    reader.readNext();
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isEndElement())  //文件结尾，退出
            break;

        if (reader.isStartElement())    //开始符号<research>，进行识别
        {
            if (reader.name() == "research")
            {
                while ( ! (reader.isEndElement() && reader.name() == "research") )  //结束符号</research>，表示research中子元素读取结束
                {
                    //qDebug()<<" Beam: research reader name ="<<reader.name();
                    if (reader.name() == "afcp")
                    {
                        item_has[0] = true;
                        afcp[RESEARCH] = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "inje")
                    {
                        item_has[1] = true;
                        inje[RESEARCH] = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "inji")
                    {
                        item_has[2] = true;
                        inji[RESEARCH] = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "elm")
                    {
                        item_has[3] = true;
                        elm[RESEARCH] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "bmi")
                    {
                        item_has[4] = true;
                        bmi[RESEARCH] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "stc")
                    {
                        item_has[5] = true;
                        stc[RESEARCH] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "len")
                    {
                        item_has[6] = true;
                        len[RESEARCH] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "pfn")
                    {
                        item_has[7] = true;
                        pfn[RESEARCH] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "prfp")
                    {
                        item_has[8] = true;
                        prfp[RESEARCH] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "ipfn")
                    {
                        item_has[9] = true;
                        ipfn[RESEARCH] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "somin")
                    {
                        item_has[10] = true;
                        somin[RESEARCH] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "bmismin")
                    {
                        item_has[11] = true;
                        bmisMin[RESEARCH] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "bmismax")
                    {
                        item_has[12] = true;
                        bmisMax[RESEARCH] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "elmck")
                    {
                        item_has[13] = true;
                        elmck[RESEARCH] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "bmick")
                    {
                        item_has[14] = true;
                        bmick[RESEARCH] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "stcck")
                    {
                        item_has[15] = true;
                        stcck[RESEARCH] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "lenck")
                    {
                        item_has[16] = true;
                        lenck[RESEARCH] =  reader.readElementText().toInt();
                    }
                    reader.readNext();
                }
            }
            else if (reader.name() == "default")
            {
                while ( ! (reader.isEndElement() && reader.name() == "default") )
                {
                    //qDebug()<<" Beam: default reader name ="<<reader.name();
                    if (reader.name() == "afcp")
                    {
                        item_has[17] = true;
                        afcp[DEFAULT] = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "inje")
                    {
                        item_has[18] = true;
                        inje[DEFAULT] = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "inji")
                    {
                        item_has[19] = true;
                        inji[DEFAULT] = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "elm")
                    {
                        item_has[20] = true;
                        elm[DEFAULT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "bmi")
                    {
                        item_has[21] = true;
                        bmi[DEFAULT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "stc")
                    {
                        item_has[22] = true;
                        stc[DEFAULT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "len")
                    {
                        item_has[23] = true;
                        len[DEFAULT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "pfn")
                    {
                        item_has[24] = true;
                        pfn[DEFAULT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "prfp")
                    {
                        item_has[25] = true;
                        prfp[DEFAULT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "ipfn")
                    {
                        item_has[26] = true;
                        ipfn[DEFAULT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "somin")
                    {
                        item_has[27] = true;
                        somin[DEFAULT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "bmismin")
                    {
                        item_has[28] = true;
                        bmisMin[DEFAULT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "bmismax")
                    {
                        item_has[29] = true;
                        bmisMax[DEFAULT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "elmck")
                    {
                        item_has[30] = true;
                        elmck[DEFAULT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "bmick")
                    {
                        item_has[31] = true;
                        bmick[DEFAULT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "stcck")
                    {
                        item_has[32] = true;
                        stcck[DEFAULT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "lenck")
                    {
                        item_has[33] = true;
                        lenck[DEFAULT] =  reader.readElementText().toInt();
                    }
                    reader.readNext();
                }
            }
            else if (reader.name() == "treatment")
            {
                while ( ! (reader.isEndElement() && reader.name() =="treatment") )
                {
                    //qDebug()<<" Beam: treatment reader name ="<<reader.name();
                    if (reader.name() == "afcp")
                    {
                        item_has[34] = true;
                        afcp[TREATMENT] = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "inje")
                    {
                        item_has[35] = true;
                        inje[TREATMENT] = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "inji")
                    {
                        item_has[36] = true;
                        inji[TREATMENT] = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "elm")
                    {
                        item_has[37] = true;
                        elm[TREATMENT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "bmi")
                    {
                        item_has[38] = true;
                        bmi[TREATMENT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "stc")
                    {
                        item_has[39] = true;
                        stc[TREATMENT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "len")
                    {
                        item_has[40] = true;
                        len[TREATMENT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "pfn")
                    {
                        item_has[41] = true;
                        pfn[TREATMENT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "prfp")
                    {
                        item_has[42] = true;
                        prfp[TREATMENT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "ipfn")
                    {
                        item_has[43] = true;
                        ipfn[TREATMENT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "somin")
                    {
                        item_has[44] = true;
                        somin[TREATMENT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "bmismin")
                    {
                        item_has[45] = true;
                        bmisMin[TREATMENT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "bmismax")
                    {
                        item_has[46] = true;
                        bmisMax[TREATMENT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "elmck")
                    {
                        item_has[47] = true;
                        elmck[TREATMENT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "bmick")
                    {
                        item_has[48] = true;
                        bmick[TREATMENT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "stcck")
                    {
                        item_has[49] = true;
                        stcck[TREATMENT] =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "lenck")
                    {
                        item_has[50] = true;
                        lenck[TREATMENT] =  reader.readElementText().toInt();
                    }
                    reader.readNext();
                }
            }
        }
    }
    return true;
}

//写入参数到文件，与读取方式不同，使用QDomNode方式
void Beam::WriteElement()
{
    QDomNodeList list_l1, list_l2;
    QDomNode node_l1, node_l2;
    qDebug()<<"beam WriteElement";
    list_l1 = node.childNodes();    //用QDomNodeList获取子节点
    for (int i = 0; i < list_l1.count(); i++)   //遍历list获取子节点
    {
        node_l1 = list_l1.at(i);
        if (node_l1.nodeName() == "research")
        {
            list_l2 = node_l1.childNodes();     //若节点还有子节点，再用list来获取
            for (int j = 0; j < list_l2.count(); j++)
            {
                node_l2 = list_l2.at(j);        //遍历子节点对内存中的node赋值
                if (node_l2.nodeName() == "afcp")
                    node_l2.firstChild().setNodeValue(QString::number(afcp[RESEARCH])); //node的firstchild就是value
                else if (node_l2.nodeName() == "inje")
                    node_l2.firstChild().setNodeValue(QString::number(inje[RESEARCH]));
                else if (node_l2.nodeName() == "inji")
                    node_l2.firstChild().setNodeValue(QString::number(inji[RESEARCH]));
                else if (node_l2.nodeName() == "elm")
                    node_l2.firstChild().setNodeValue(QString::number(elm[RESEARCH]));
                else if (node_l2.nodeName() == "bmi")
                    node_l2.firstChild().setNodeValue(QString::number(bmi[RESEARCH]));
                else if (node_l2.nodeName() == "stc")
                    node_l2.firstChild().setNodeValue(QString::number(stc[RESEARCH]));
                else if (node_l2.nodeName() == "len")
                    node_l2.firstChild().setNodeValue(QString::number(len[RESEARCH]));
                else if (node_l2.nodeName() == "pfn")
                    node_l2.firstChild().setNodeValue(QString::number(pfn[RESEARCH]));
                else if (node_l2.nodeName() == "prfp")
                    node_l2.firstChild().setNodeValue(QString::number(prfp[RESEARCH]));
                else if (node_l2.nodeName() == "ipfn")
                    node_l2.firstChild().setNodeValue(QString::number(ipfn[RESEARCH]));
                else if (node_l2.nodeName() == "somin")
                    node_l2.firstChild().setNodeValue(QString::number(somin[RESEARCH]));
                else if (node_l2.nodeName() == "bmismin")
                    node_l2.firstChild().setNodeValue(QString::number(bmisMin[RESEARCH]));
                else if (node_l2.nodeName() == "bmismax")
                    node_l2.firstChild().setNodeValue(QString::number(bmisMax[RESEARCH]));
                else if (node_l2.nodeName() == "elmck")
                    node_l2.firstChild().setNodeValue(QString::number(elmck[RESEARCH]));
                else if (node_l2.nodeName() == "bmick")
                    node_l2.firstChild().setNodeValue(QString::number(bmick[RESEARCH]));
                else if (node_l2.nodeName() == "stcck")
                    node_l2.firstChild().setNodeValue(QString::number(stcck[RESEARCH]));
                else if (node_l2.nodeName() == "lenck")
                    node_l2.firstChild().setNodeValue(QString::number(lenck[RESEARCH]));
            }
        }
        else if (node_l1.nodeName() == "default")
        {
            list_l2 = node_l1.childNodes();
            for (int j = 0; j < list_l2.count(); j++)
            {
                node_l2 = list_l2.at(j);
                if (node_l2.nodeName() == "afcp")
                    node_l2.firstChild().setNodeValue(QString::number(afcp[DEFAULT]));
                else if (node_l2.nodeName() == "inje")
                    node_l2.firstChild().setNodeValue(QString::number(inje[DEFAULT]));
                else if (node_l2.nodeName() == "inji")
                    node_l2.firstChild().setNodeValue(QString::number(inji[DEFAULT]));
                else if (node_l2.nodeName() == "elm")
                    node_l2.firstChild().setNodeValue(QString::number(elm[DEFAULT]));
                else if (node_l2.nodeName() == "bmi")
                    node_l2.firstChild().setNodeValue(QString::number(bmi[DEFAULT]));
                else if (node_l2.nodeName() == "stc")
                    node_l2.firstChild().setNodeValue(QString::number(stc[DEFAULT]));
                else if (node_l2.nodeName() == "len")
                    node_l2.firstChild().setNodeValue(QString::number(len[DEFAULT]));
                else if (node_l2.nodeName() == "pfn")
                    node_l2.firstChild().setNodeValue(QString::number(pfn[DEFAULT]));
                else if (node_l2.nodeName() == "prfp")
                    node_l2.firstChild().setNodeValue(QString::number(prfp[DEFAULT]));
                else if (node_l2.nodeName() == "ipfn")
                    node_l2.firstChild().setNodeValue(QString::number(ipfn[DEFAULT]));
                else if (node_l2.nodeName() == "somin")
                    node_l2.firstChild().setNodeValue(QString::number(somin[DEFAULT]));
                else if (node_l2.nodeName() == "bmismin")
                    node_l2.firstChild().setNodeValue(QString::number(bmisMin[DEFAULT]));
                else if (node_l2.nodeName() == "bmismax")
                    node_l2.firstChild().setNodeValue(QString::number(bmisMax[DEFAULT]));
                else if (node_l2.nodeName() == "elmck")
                    node_l2.firstChild().setNodeValue(QString::number(elmck[DEFAULT]));
                else if (node_l2.nodeName() == "bmick")
                    node_l2.firstChild().setNodeValue(QString::number(bmick[DEFAULT]));
                else if (node_l2.nodeName() == "stcck")
                    node_l2.firstChild().setNodeValue(QString::number(stcck[DEFAULT]));
                else if (node_l2.nodeName() == "lenck")
                    node_l2.firstChild().setNodeValue(QString::number(lenck[DEFAULT]));
            }
        }
        else if (node_l1.nodeName() == "treatment")
        {
            list_l2 = node_l1.childNodes();
            for (int j = 0; j < list_l2.count(); j++)
            {
                node_l2 = list_l2.at(j);
                if (node_l2.nodeName() == "afcp")
                    node_l2.firstChild().setNodeValue(QString::number(afcp[TREATMENT]));
                else if (node_l2.nodeName() == "inje")
                    node_l2.firstChild().setNodeValue(QString::number(inje[TREATMENT]));
                else if (node_l2.nodeName() == "inji")
                    node_l2.firstChild().setNodeValue(QString::number(inji[TREATMENT]));
                else if (node_l2.nodeName() == "elm")
                    node_l2.firstChild().setNodeValue(QString::number(elm[TREATMENT]));
                else if (node_l2.nodeName() == "bmi")
                    node_l2.firstChild().setNodeValue(QString::number(bmi[TREATMENT]));
                else if (node_l2.nodeName() == "stc")
                    node_l2.firstChild().setNodeValue(QString::number(stc[TREATMENT]));
                else if (node_l2.nodeName() == "len")
                    node_l2.firstChild().setNodeValue(QString::number(len[TREATMENT]));
                else if (node_l2.nodeName() == "pfn")
                    node_l2.firstChild().setNodeValue(QString::number(pfn[TREATMENT]));
                else if (node_l2.nodeName() == "prfp")
                    node_l2.firstChild().setNodeValue(QString::number(prfp[TREATMENT]));
                else if (node_l2.nodeName() == "ipfn")
                    node_l2.firstChild().setNodeValue(QString::number(ipfn[TREATMENT]));
                else if (node_l2.nodeName() == "somin")
                    node_l2.firstChild().setNodeValue(QString::number(somin[TREATMENT]));
                else if (node_l2.nodeName() == "bmismin")
                    node_l2.firstChild().setNodeValue(QString::number(bmisMin[TREATMENT]));
                else if (node_l2.nodeName() == "bmismax")
                    node_l2.firstChild().setNodeValue(QString::number(bmisMax[TREATMENT]));
                else if (node_l2.nodeName() == "elmck")
                    node_l2.firstChild().setNodeValue(QString::number(elmck[TREATMENT]));
                else if (node_l2.nodeName() == "bmick")
                    node_l2.firstChild().setNodeValue(QString::number(bmick[TREATMENT]));
                else if (node_l2.nodeName() == "stcck")
                    node_l2.firstChild().setNodeValue(QString::number(stcck[TREATMENT]));
                else if (node_l2.nodeName() == "lenck")
                    node_l2.firstChild().setNodeValue(QString::number(lenck[TREATMENT]));
            }
        }
    }
}

bool Beam::CheckSoftpotDataLimit()
{
    bool suc = true;
    int i;
    for(i=0;i<3;i++)    //RESEARCH=0; DEFAULT=1; TREATMENT=2
    {
        suc &= CheckXMLData(afcp[i],AFCP_RSCH_CK);
        suc &= CheckXMLData(inje[i],INJE_RSCH_CK);
        suc &= CheckXMLData(inji[i],INJI_RSCH_CK);
        suc &= CheckXMLData(elm[i],ELM_RSCH_CK);
        suc &= CheckXMLData(bmi[i],BMI_RSCH_CK);
        suc &= CheckXMLData(stc[i],STC_RSCH_CK);
        suc &= CheckXMLData(len[i],LENS_RSCH_CK);
        suc &= CheckXMLData(pfn[i],PFN_RSCH_CK);
        suc &= CheckXMLData(ipfn[i],IPFN_RSCH_CK);
        suc &= CheckXMLData(prfp[i],PRFP_CK);
        suc &= CheckXMLData(somin[i],SOMIN_RSCH_CK);
//        suc &= CheckValue(bmisMin[i],);
//        suc &= CheckValue(bmisMax[i],);
        suc &= CheckXMLData(elmck[i],ELMCK_RSCH_CK);
        suc &= CheckXMLData(bmick[i],BMICK_RSCH_CK);
        suc &= CheckXMLData(stcck[i],STCCK_RSCH_CK);
        suc &= CheckXMLData(lenck[i],LENCK_RSCH_CK);
    }
    suc &= CheckReschTreatToDefault(TREATMENT);
    return suc;
}

bool Beam::CheckReschTreatToDefault(RE_TE_MODE mode)
{
    if( afcp[mode]<(afcp[DEFAULT]*0.9) || afcp[mode]>(afcp[DEFAULT]*1.1)) return false;
    if( inje[mode]<(inje[DEFAULT]*0.9) || inje[mode]>(inje[DEFAULT]*1.1) ) return false;
    if( inji[mode]<(inji[DEFAULT]*0.9) || inji[mode]>(inji[DEFAULT]*1.1) ) return false;
    if( elm[mode]<(elm[DEFAULT]*0.9) || elm[mode]>(elm[DEFAULT]*1.1) ) return false;
    if( bmi[mode]<(bmi[DEFAULT]*0.9) || bmi[mode]>(bmi[DEFAULT]*1.1) ) return false;
    if( stc[mode]<(stc[DEFAULT]*0.9) || stc[mode]>(stc[DEFAULT]*1.1) ) return false;
    if( len[mode]<(len[DEFAULT]*0.9) || len[mode]>(len[DEFAULT]*1.1) ) return false;
    if( pfn[mode]<(pfn[DEFAULT]*0.9) || pfn[mode]>(pfn[DEFAULT]*1.1) ) return false;
    if( ipfn[mode]<(ipfn[DEFAULT]*0.9) || ipfn[mode]>(ipfn[DEFAULT]*1.1) ) return false;
    if( prfp[mode]<(prfp[DEFAULT]*0.9) || prfp[mode]>(prfp[DEFAULT]*1.1) ) return false;
    if( somin[mode]<(somin[DEFAULT]*0.9) || somin[mode]>(somin[DEFAULT]*1.1) ) return false;
    if( elmck[mode]<(elmck[DEFAULT]*0.9) || elmck[mode]>(elmck[DEFAULT]*1.1) ) return false;
    if( bmick[mode]<(bmick[DEFAULT]*0.9) || bmick[mode]>(bmick[DEFAULT]*1.1) ) return false;
    if( stcck[mode]<(stcck[DEFAULT]*0.9) || stcck[mode]>(stcck[DEFAULT]*1.1) ) return false;
    if( lenck[mode]<(lenck[DEFAULT]*0.9) || lenck[mode]>(lenck[DEFAULT]*1.1) ) return false;
    return true;
}

bool Beam::CheckSoftpotDataIntegrity()
{
    for(int i=0; i<51; i++)
    {
        if(!item_has[i])
        {
            qDebug()<<"beam softpot integrity error:  i ="<<i;
            return false;
        }
    }
    return true;
}

//计算倒数第二位的校验码
//方法：高4位为模块私有技术0 - 15
//  低4位为长度异或各数据位，最后高低位异或
bool Beam::ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc)
{
    UINT8 low = 0, tmplow = 0;
    if ( len > 18)
        return false;

    fc = send_frame_num << 4;
    send_frame_num = (send_frame_num + 1) % 16;

    low = data[0] ^ len;
    for (int i = 1; i < len; i++)
        low = low ^ data[i];

    tmplow = (low & 0x0F) ^ ((low & 0xF0) >> 4);
    fc += tmplow;

    return true;
}

//计算收到的倒数第二位校验和
UINT8 Beam::ComputeRevCHK(UINT8* data, UINT8 len)
{
    UINT8 fc;
    UINT8 low = 0, tmplow = 0;

    fc = rev_frame_num << 4;
    rev_frame_num = (rev_frame_num + 1) % 16;

    low = data[0] ^ len;
    for (int i = 1; i < len; i++)
        low = low ^ data[i];

    tmplow = (low & 0x0F) ^ ((low & 0xF0) >> 4);
    fc += tmplow;

    return fc;
}

bool Beam::SendC0()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = 0x23;
    tmpSend.data[1] = 0xc0;
    tmpSend.data[2] = 0x00;
    tmpSend.data[3] = 0xe3;
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

//发送31(1)命令
bool Beam::SendAfcpInjeInji()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof(TransRecvT));
    tmpSend.data[0] = TOBEAM;
    tmpSend.data[1] = 0x31;
    tmpSend.data[2] = (UINT8)(afcp[g_mode] & 0x00ff);   //高位在前低位在后
    tmpSend.data[3] = (UINT8)(afcp[g_mode] >> 8);
    tmpSend.data[4] = (UINT8)(inje[g_mode] & 0x00ff);
    tmpSend.data[5] = (UINT8)(inje[g_mode] >> 8);
    tmpSend.data[6] = (UINT8)(inji[g_mode] & 0x00ff);
    tmpSend.data[7] = (UINT8)(inji[g_mode] >> 8);
    tmpSend.data[8] = 0x00;
    tmpSend.data[9] = 0x00;

    QMutexLocker locker(&beam_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 10, tmpSend.data[10]))
        return false;
    tmpSend.data[11] = ComputeLstCHK(&tmpSend.data[0], 11);
    tmpSend.len = 12;

    return InsertOutQueue(tmpSend);

}

bool Beam::SendElmBmiStcLen()
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOBEAM;
    tmpSend.data[1]=0x32;
    tmpSend.data[2]=(UINT8)(elm[g_mode]&0x00ff);
    tmpSend.data[3]=(UINT8)(elm[g_mode]>>8);
    tmpSend.data[4]=(UINT8)(bmi[g_mode]&0x00ff);
    tmpSend.data[5]=(UINT8)(bmi[g_mode]>>8);
    tmpSend.data[6]=(UINT8)(stc[g_mode]&0x00ff);
    tmpSend.data[7]=(UINT8)(stc[g_mode]>>8);
    tmpSend.data[8]=(UINT8)(len[g_mode]&0x00ff);
    tmpSend.data[9]=(UINT8)(len[g_mode]>>8);
    QMutexLocker locker(&beam_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 10, tmpSend.data[10]))
        return false;
    tmpSend.data[11]=ComputeLstCHK(&tmpSend.data[0], 11);
    tmpSend.len = 12;

    return InsertOutQueue(tmpSend);
}

bool Beam::SendPfnPrfpIpfnSomin()
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOBEAM;
    tmpSend.data[1]=0x33;
    tmpSend.data[2]=(UINT8)(pfn[g_mode]&0x00ff);
    tmpSend.data[3]=(UINT8)(pfn[g_mode]>>8);
    tmpSend.data[4]=(UINT8)(prfp[g_mode]&0x00ff);
    tmpSend.data[5]=(UINT8)(prfp[g_mode]>>8);
    tmpSend.data[6]=(UINT8)(ipfn[g_mode]&0x00ff);
    tmpSend.data[7]=(UINT8)(ipfn[g_mode]>>8);
    tmpSend.data[8]=(UINT8)(somin[g_mode]&0x00ff);
    tmpSend.data[9]=(UINT8)(somin[g_mode]>>8);
    QMutexLocker locker(&beam_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 10, tmpSend.data[10]))
        return false;
    tmpSend.data[11]=ComputeLstCHK(&tmpSend.data[0], 11);
    tmpSend.len = 12;

    return InsertOutQueue(tmpSend);
}

bool Beam::SendDataNotUsed()
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));

    tmpSend.data[0] = TOBEAM;
    tmpSend.data[1] = 0x34;
    tmpSend.data[2] = 0;
    tmpSend.data[3] = 0;
    tmpSend.data[4] = 0;
    tmpSend.data[5] = 0;
    tmpSend.data[6] = 0;
    tmpSend.data[7] = 0;

    QMutexLocker locker(&beam_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 8, tmpSend.data[8]))
        return false;
    tmpSend.data[9] = ComputeLstCHK(&tmpSend.data[0] , 9);
    tmpSend.len = 10;

    return InsertOutQueue(tmpSend);
}

bool Beam::SendBmisMinMax()
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0] = TOBEAM;
    tmpSend.data[1] = 0x35;
    bmisMin[g_mode] =  (UINT16)(bmick[g_mode] * 0.95);   // 100A =4095(0x7FF), 0A= 0x000, bmick[g_mode] is Amp
    bmisMax[g_mode] =  (UINT16)(bmick[g_mode] * 1.05);
    tmpSend.data[2] = (UINT8)(bmisMin[g_mode] & 0x00ff);
    tmpSend.data[3] = (UINT8)(bmisMin[g_mode] >> 8);
    tmpSend.data[4] = (UINT8)(bmisMax[g_mode] & 0x00ff);
    tmpSend.data[5] = (UINT8)(bmisMax[g_mode] >> 8);

    QMutexLocker locker(&beam_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 6, tmpSend.data[6]))
        return false;
    tmpSend.data[7]=ComputeLstCHK(&tmpSend.data[0],7);
    tmpSend.len = 8;

    return InsertOutQueue(tmpSend);
}

bool Beam::SendUserMode(USERMODE_TYPE_T slectmode)
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOBEAM;
    tmpSend.data[1]=0x43;
    tmpSend.data[2]=slectmode;
    QMutexLocker locker(&beam_mutex);
    if(slectmode==RESET_MODE)//复位后帧计数清零
    {
        send_frame_num=0;
        //rev_frame_num=0;
    }
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4]=ComputeLstCHK(&tmpSend.data[0],4);
    tmpSend.len = 5;
    return InsertOutQueue(tmpSend);
}

bool Beam::SendAccept()
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOBEAM;
    tmpSend.data[1]=0x49;
    QMutexLocker locker(&beam_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3]=ComputeLstCHK(&tmpSend.data[0],3);

    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool Beam::SendRadOn()
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOBEAM;
    tmpSend.data[1]=0x52;
    QMutexLocker locker(&beam_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3]=ComputeLstCHK(&tmpSend.data[0],3);
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool Beam::SendInterupt()
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOBEAM;
    tmpSend.data[1]=0x4f;
    QMutexLocker locker(&beam_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3]=ComputeLstCHK(&tmpSend.data[0],3);
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool Beam::SendTerminateOrComplete()
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOBEAM;
    tmpSend.data[1]=0x54;
    QMutexLocker locker(&beam_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3]=ComputeLstCHK(&tmpSend.data[0],3);
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool Beam::SendCommDog()
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOBEAM;
    tmpSend.data[1]=0x46;
    QMutexLocker locker(&beam_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3]=ComputeLstCHK(&tmpSend.data[0],3);
    tmpSend.len = 4;


    return InsertOutQueue(tmpSend);
}

bool Beam::SendCheckRequest(CHECKREQ_TYPE_T check)
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOBEAM;
    tmpSend.data[1]=0x57;
    tmpSend.data[2]=check;
    QMutexLocker locker(&beam_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4]=ComputeLstCHK(&tmpSend.data[0],4);
    tmpSend.len = 5;
    return InsertOutQueue(tmpSend);
}

bool Beam::SendReadyStatus(READYSTATE_TYPE_T state)
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOBEAM;
    tmpSend.data[1]=0x5a;
    tmpSend.data[2]=state;
    QMutexLocker locker(&beam_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4]=ComputeLstCHK(&tmpSend.data[0],4);
    tmpSend.len = 5;
    return InsertOutQueue(tmpSend);
}

bool Beam::SendClear()
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOBEAM;
    tmpSend.data[1]=0x4c;
    QMutexLocker locker(&beam_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3]=ComputeLstCHK(&tmpSend.data[0],3);
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

//分析收到的命令
bool Beam::AnalysRevFram(TransRecvT& revdata)
{
    if (revdata.data[1]=='C')
        rev_frame_num = 0;

    if (!ProcessCHKIL(revdata))
        return false;

    switch(revdata.data[1]) //根据第2（data[1]）个字节来判断命令的类型
    {
    case '1':
        return RevAfcmBeamiInjpiPusm(revdata);
    case '2':
        return RevElectBendSteerLen(revdata);
    case '3':
        return RevPulseVoltage(revdata);
    case 'E':
        return RevErrorReport(revdata);
    case 'F':
        return RevCommDog(revdata);
    case 'Y':
        return RevEmergenLineStatus(revdata);
    case 'W':
        return RevCheckReport(revdata);
    case 'C':
        return RevResetEcho(revdata);
    default:   //Invalid command type received from controller 3
        InsertInterlock(79, 953, ERROR_IL);
    }

    return false;
}

bool Beam::RevAfcmBeamiInjpiPusm(TransRecvT &revdata)
{
    //    float tmp ;
    if (revdata.len != 12)  //判断长度是否符合要求
    {
        InsertInterlock(79, 963, ERROR_IL);   //Invalid length for command type from controller 3 .
        return false;
    }

    afcm = (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00);
    beami = (revdata.data[4] & 0x00FF) | ((revdata.data[5] << 8) & 0xFF00);
    injpi = (revdata.data[6] & 0x00FF) | ((revdata.data[7] << 8) & 0xFF00);
    phsm = (revdata.data[8] & 0x00FF) | ((revdata.data[9] << 8) & 0xFF00);

    //    tmp = ConvertMemberToFloat(afcm, AFCP_RSCH_CK);
    //    accui->SetActAfcm((UINT32)tmp);
    emit base->ShowActSvcValueSig(AFCM);

    return true;

}

bool Beam::RevElectBendSteerLen(TransRecvT &revdata)
{
    //    float tmp;

    if (revdata.len != 12)
    {
        InsertInterlock(79, 963, ERROR_IL);   //Invalid length for command type from controller 3 .
        return false;
    }

    elmi = (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00);
    elmi_f=((float) elmi) *100.0/4095.0;
    bmis = (revdata.data[4] & 0x00FF) | ((revdata.data[5] << 8) & 0xFF00);
    bmis_f=((float)bmis) * 100.0/4095.0;
    stci = (revdata.data[6] & 0x00FF) | ((revdata.data[7] << 8) & 0xFF00);
    stci_f=((float)stci) * 10000.0/4095;
    leni = (revdata.data[8] & 0x00FF) | ((revdata.data[9] << 8) & 0xFF00);
    leni_f=((float)leni) * 10.0 /4095.0;

    emit base->ShowActSvcValueSig(ELMI_BMIS_STCI_LENI);

    ProcessElectBendSteerLenIL();
    return true;
}

bool Beam::RevPulseVoltage(TransRecvT &revdata)
{
    if (revdata.len != 10)
    {
        InsertInterlock(79, 963, ERROR_IL);   //Invalid length for command type from controller 3 .
        return false;
    }

    pfne = (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00);

    return true;
}

bool Beam::RevErrorReport(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 963, ERROR_IL);   //Invalid length for command type from controller 3 .
        return false;
    }

    error_now = revdata.data[2];

    ProcessErrorReportIL();
    return true;
}

bool Beam::RevCommDog(TransRecvT &revdata)
{
    if (revdata.len != 4)
    {
        InsertInterlock(79, 963, ERROR_IL);   //Invalid length for command type from controller 3 .
        return false;
    }

    // g_commdog = true;
    if (g_status==RAD_ON)
    {
        emit base->g_commdog_start_signal();
    }

    return true;
}

bool Beam::RevEmergenLineStatus(TransRecvT& revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 963, ERROR_IL);   //Invalid length for command type from controller 3 .
        return false;
    }

    line_status = revdata.data[2];
    //    accui->SetActT1DispReport3(line_status);
    emit base->ShowActSvcValueSig(T1_3);

    return true;
}
bool Beam::RevCheckReport(TransRecvT& revdata)
{
    CHECKREQ_TYPE_T check_status;
    if (revdata.len != 5)
    {
        InsertInterlock(79, 963, ERROR_IL);   //Invalid length for command type from controller 3 .
        return false;
    }

    check_report = revdata.data[2];

    g_check_index_mutex.lock();
    check_status = g_check_index;
    g_check_index_mutex.unlock();

    if(check_status==CHECK2)
    {
        //        accui->SetActChkDispReport3(check_report);
        emit base->ShowActSvcValueSig(CHK_3);
    }

    if (check_report == 0x0)
    {
        g_check_bits_mutex.lock();
        g_check_bits.setBit(3, true);
        g_check_bits_mutex.unlock();
    }
    else if (check_report == 0x1)
    {
        //interlock
        if (check_status == CHECK1)
        {
            ;
        }
        else if (check_status == CHECK2)
        {
            ;
        }
    }
    else if (check_report == 0x2)
    {
        //interlock
        ;
    }
    else if (check_report == 0x40)
    {
        //interlock
    }

    return true;
}

bool Beam::RevResetEcho(TransRecvT& revdata)
{

    if (revdata.len != 9)
    {
        InsertInterlock(79, 963, ERROR_IL);   //Invalid length for command type from controller 3 .
        return false;
    }

    if (revdata.data[2] != 0)
        return false;
    memcpy(&prom_version, &(revdata.data[3]), sizeof (prom_version));
    g_c0_echo_bits_lock.lockForWrite();
    g_c0_echo_bits.setBit(3, true);
    g_c0_echo_bits_lock.unlock();

    return true;
}

void Beam::ProcessElectBendSteerLenIL()
{
    //安全联锁
    //qDebug()<<"leni="<<leni_f<<", lenck_f="<<lenck_f[g_mode];
    if ((leni_f>lenck_f[g_mode]*1.15) || (leni_f<lenck_f[g_mode]*0.85))
        InsertInterlock(16, 858, SAFETY_IL, IL_READY | IL_INTERRUPT | IL_RADON,
                        NOT_LATCHED);
    else
        DeleteInterlock(16, 858);

    if ((stci_f> stcck_f[g_mode]*1.15) || (stci_f < stcck_f[g_mode]*0.85))     //Console detected LENI versus LENCK mismatch
            InsertInterlock(17, 857, SAFETY_IL, IL_READY|IL_INTERRUPT|IL_RADON,
                            NOT_LATCHED);
    else
        DeleteInterlock(17, 857);

    if (elmi_f>elmck_f[g_mode]*1.05 || elmi_f<elmck_f[g_mode]*0.95 )
        InsertInterlock(18, 855, SAFETY_IL, IL_READY|IL_INTERRUPT|IL_RADON,
                        NOT_LATCHED);
    else
        DeleteInterlock(18, 855);

    if (bmis_f> bmick_f[g_mode] * 1.05 || bmis_f< bmick_f[g_mode] *0.95)
        InsertInterlock(35, 856, SAFETY_IL, IL_READY|IL_INTERRUPT|IL_RADON,
                        NOT_LATCHED);
    else
        DeleteInterlock(35, 856);
}

void Beam::ProcessErrorReportIL()
{
    switch (error_now)
    {
    case 0x5:     //bending magnent 安全联锁
        InsertInterlock(35, 305, SAFETY_IL, IL_RADON, LATCHED);
        break;
    case 0x6:       //Missing HV-ON singal after RAD-ON command .未知
        InsertInterlock(66, 306, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x7:       //Wartchdog (‘F’) command not received from console for 3 seconds 安全联锁
        InsertInterlock(66, 307, ERROR_IL, IL_RADON, LATCHED);
        break;
    case 0x8:       //HV_ON , RAD_ON ,and/or GANTRY_EN erroneously active  check#1 to not ready
        InsertInterlock(66, 308, ERROR_IL, IL_CHECK1, LATCHED);
        break;
    case 0x9:       //Segment 0 feedback value <segment 0 minimum limit
        InsertInterlock(66, 309, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x15:      //Ram test failed
        InsertInterlock(66, 321, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x16:      //Rom test failded
        InsertInterlock(66, 322, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x17:      //RAM test and ROM test failed
        InsertInterlock(66, 323, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x1E:      //Wrong DIP switch setting on controller 3 PCB
        InsertInterlock(66, 330, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x61:      //Frame-# and checksum received by controller 3 are wrong
        InsertInterlock(79, 397, ERROR_IL);
        break;
    case 0x62:      // Checksum received by controller 3 is wrong
        InsertInterlock(79, 398, ERROR_IL);
        break;
    case 0x63:      //Frame-# received by controller 3 is wrong
        InsertInterlock(79, 399, ERROR_IL);
        break;
    case 0x32:      //Dose 1 vs . console time synch error .
        InsertInterlock(80, 350, SYSTEM_IL, IL_READY|IL_INTERRUPT,
                        NOT_LATCHED);
        break;
    default:        //Invalid data received from controller 3 安全联锁
        InsertInterlock(79, 973, ERROR_IL);
        break;
    }
}

bool Beam::ProcessCHKIL(TransRecvT &revdata)
{
    UINT8 fc;
    if ((ComputeLstCHK(revdata.data, revdata.len - 1) & 0x7F) !=
            (revdata.data[revdata.len - 1] & 0x7F))
    {
        qDebug()<<"Beam lstCHK error";
        return false;
    }
    fc = ComputeRevCHK(revdata.data, revdata.len - 2);
    if (fc != revdata.data[revdata.len - 2])
    {
        if ((fc & 0xF0) != (revdata.data[revdata.len - 2] & 0xF0))
        {
            qDebug()<<"Beam: Invalid frame number, the console's rev_frame_number is"<<rev_frame_num;
            InsertInterlock(79, 933, ERROR_IL);       // Invalid frame number detected from controller 3 安全联锁
        }
        if ((fc & 0x0F) !=  (revdata.data[revdata.len - 2] & 0x0F))
        {
            qDebug()<<"Beam: Invalid checksum detected";
            InsertInterlock(79, 943, ERROR_IL);       //Invalid checksum detected from controller 3 安全联锁
        }
        return false;
    }
    return true;
}


