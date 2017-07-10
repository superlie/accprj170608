#include "motor.h"
#include "can/Can.h"
#include "ui/accui.h"

extern AccUi *accui;

Motor::Motor()
{
    if (!ReadOwnFile())
    {
        ;//interlock
    }

    //LJY-12-13
    init_jawcolli_target_pos = false;
    init_gantry_target_pos = false;

    g_edge_arr[JAWY1_CK].k = 1 / jaw_y1_gain;
    g_edge_arr[JAWY2_CK].k = 1 / jaw_y2_gain;
    g_edge_arr[JAWX2_CK].k = 1 / jaw_x2_gain;
    g_edge_arr[JAWX1_CK].k = 1 / jaw_x1_gain;
    g_edge_arr[JAWY1_CK].b = (11 - jaw_y1_offset) / jaw_y1_gain;
    g_edge_arr[JAWY2_CK].b = (11 - jaw_y2_offset) / jaw_y2_gain;
    g_edge_arr[JAWX2_CK].b = (3 - jaw_x2_offset) / jaw_x2_gain;
    g_edge_arr[JAWX1_CK].b = (3 - jaw_x1_offset) / jaw_x1_gain;

    g_edge_arr[COLLI_CK].k = 1 / colli_gain;
    g_edge_arr[COLLI_CK].b = - colli_offset / colli_gain;
    g_edge_arr[GANTRY_CK].k = - 1 / gantry_gain;
    g_edge_arr[GANTRY_CK].b = (190 - gantry_offset) / gantry_gain;
    //qDebug()<<"Gantry1 k="<<g_edge_arr[GANTRY_CK].k<<"  b="<<g_edge_arr[GANTRY_CK].b;

//    if (colli_type == COLLI_180)
//    {
        g_edge_arr[COLLI_CK].max_value = 95.0;
        g_edge_arr[COLLI_CK].min_value = -95.0;    //mark 270映射到-90.0
//    }
//    else if (colli_type == COLLI_360)
//    {
//        g_edge_arr[COLLI_CK].max_value = colli_offset;
//        g_edge_arr[COLLI_CK].min_value = colli_offset - 360 + 0.1;
//    }

    for (int i = 0; i < 5; i++)
        position[i] = 0;

    for(int i=0;i<12;i++)
        gain_offset_has[i] = false;

    //Motor Speed test ID, position & timestamp
    //Motor Speed Test Complete
    for (int i = 0; i < 6; i++)
    {
        speed_test[i].position = 0;
        speed_test[i].timestamp = 0;
        speed_complete[i].actual_position = 0;
        speed_complete[i].requested_position = 0;
    }

    //Virtual Wedge Motor Speed Calibration Test Complete
    for (int i = 0; i < 4; i++)
    {
        speed_virtual_wedge_complete[i].actual_position = 0;
        speed_virtual_wedge_complete[i].requested_position = 0;
    }
    mode_now = RESET_MODE;
    ready_status_now = NOTREADY;
    prom_version = 0;   //版本号等信息

    //    memset(&error_mask_1, 0, sizeof(error_mask_1));
    //    memset(&error_mask_2, 0, sizeof(error_mask_2));

    //error mask for command D
    error_mask_1.error_mask_bytes[0] = 0x00;
    error_mask_1.error_mask_bytes[1] = 0x00;
    error_mask_1.error_mask_bytes[2] = 0x00;
    error_mask_1.error_mask_bytes[3] = 0x00;
    error_mask_1.error_mask_bytes[4] = 0x00;
    error_mask_1.error_mask_bytes[5] = 0x00;
    error_mask_1.error_mask_bytes[6] = 0x00;

    error_mask_2.error_mask_bytes[0] = 0x00;
    error_mask_2.error_mask_bytes[1] = 0x00;
    error_mask_2.error_mask_bytes[2] = 0x00;
    error_mask_2.error_mask_bytes[3] = 0x00;
    error_mask_2.error_mask_bytes[4] = 0x00;
    error_mask_2.error_mask_bytes[5] = 0x00;
    error_mask_2.error_mask_bytes[6] = 0x00;

    memset(&gantry_position_speed, 0, sizeof(gantry_position_speed));
    //Gantry completion in RAD ON   cmd'K'
    memset(&gantry_completion, 0, sizeof(gantry_completion));

    //刚刚报告的错误
    memset(&error_now, 0, sizeof(error_now));
    check_report = CHECKPASSED;

    line_status = T1_ACTIVE_LOW;            //Emergency line status
    check_index = CHECK1;

    send_frame_num = 0;
    rev_frame_num = 0;


    UINT32 tmp1,tmp2;

    SlaveBase::CheckValue(0.3f, JAWX1_CK, tmp1);
    SlaveBase::CheckValue(0, JAWX1_CK, tmp2);
    jawx1_max_toler = qAbs((UINT16)tmp1 - (UINT16)tmp2);

    SlaveBase::CheckValue(0.3f, JAWX2_CK, tmp1);
    SlaveBase::CheckValue(0, JAWX2_CK, tmp2);
    jawx2_max_toler = qAbs((UINT16)tmp1 - (UINT16)tmp2);

    SlaveBase::CheckValue(0.3f, JAWY1_CK, tmp1);
    SlaveBase::CheckValue(0, JAWY1_CK, tmp2);
    jawy1_max_toler = qAbs((UINT16)tmp1 - (UINT16)tmp2);

    SlaveBase::CheckValue(0.3f, JAWY2_CK, tmp1);
    SlaveBase::CheckValue(0, JAWY2_CK, tmp2);
    jawy2_max_toler = qAbs((UINT16)tmp1 - (UINT16)tmp2);

    SlaveBase::CheckValue(2, COLLI_CK, tmp1);
    SlaveBase::CheckValue(1, COLLI_CK, tmp2);
    colli_max_toler = qAbs((UINT16)tmp1 - (UINT16)tmp2);
    //qDebug()<<"tmp1="<<tmp1<<", tmp2="<<tmp2;

    SlaveBase::CheckValue(2, GANTRY_CK, tmp1);
    SlaveBase::CheckValue(1, GANTRY_CK, tmp2);
    gantry_max_toler = qAbs((UINT16)tmp1 - (UINT16)tmp2);
    //qDebug()<<"tmp1="<<tmp1<<", tmp2="<<tmp2;

    qDebug()<<"toler: x1"<<jawx1_max_toler<<", x2"<<jawx2_max_toler<<", y1"<<jawy1_max_toler
              <<", y2"<<jawy2_max_toler<<", colli"<<colli_max_toler<<", gantry"<<gantry_max_toler;

    SlaveBase::CheckValue(15.3f, JAWX1_CK, jawx1_15_3_pos);
    SlaveBase::CheckValue(15.3f, JAWX2_CK, jawx2_15_3_pos);
    SlaveBase::CheckValue(10.3f, JAWY1_CK, jawy1_10_3_pos);
    SlaveBase::CheckValue(10.3f, JAWY2_CK, jawy2_10_3_pos);
    SlaveBase::CheckValue(12.8f, JAWY1_CK, jawy1_12_8_pos);
    SlaveBase::CheckValue(12.8f, JAWY2_CK, jawy2_12_8_pos);
    qDebug()<<jawx1_15_3_pos<<jawx2_15_3_pos<<jawy1_10_3_pos<<jawy2_10_3_pos
              <<jawy1_12_8_pos<<jawy2_12_8_pos;

   //duanzhibin
    SlaveBase::CheckValue(6.5f, JAWX1_CK, jawx1_6_5_pos);
    SlaveBase::CheckValue(6.5f, JAWX2_CK, jawx2_6_5_pos);
    SlaveBase::CheckValue(6.5f, JAWY1_CK, jawy1_6_5_pos);
    SlaveBase::CheckValue(6.5f, JAWY2_CK, jawy2_6_5_pos);

    SlaveBase::CheckValue(9.5f, JAWX1_CK, jawx1_9_5_pos);
    SlaveBase::CheckValue(9.5f, JAWX2_CK, jawx2_9_5_pos);
    SlaveBase::CheckValue(9.5f, JAWY1_CK, jawy1_9_5_pos);
    SlaveBase::CheckValue(9.5f, JAWY2_CK, jawy2_9_5_pos);

    SlaveBase::CheckValue(11.5f, JAWX1_CK, jawx1_11_5_pos);
    SlaveBase::CheckValue(11.5f, JAWX2_CK, jawx2_11_5_pos);
    SlaveBase::CheckValue(11.5f, JAWY1_CK, jawy1_11_5_pos);
    SlaveBase::CheckValue(11.5f, JAWY2_CK, jawy2_11_5_pos);

    SlaveBase::CheckValue(13.5f, JAWX1_CK, jawx1_13_5_pos);
    SlaveBase::CheckValue(13.5f, JAWX2_CK, jawx2_13_5_pos);
    SlaveBase::CheckValue(13.5f, JAWY1_CK, jawy1_13_5_pos);
    SlaveBase::CheckValue(13.5f, JAWY2_CK, jawy2_13_5_pos);

    SlaveBase::CheckValue(16, JAWX1_CK, jawx1_16_pos);
    SlaveBase::CheckValue(16, JAWX2_CK, jawx2_16_pos);
    SlaveBase::CheckValue(16, JAWY1_CK, jawy1_16_pos);
    SlaveBase::CheckValue(16, JAWY2_CK, jawy2_16_pos);
}

Motor::~Motor()
{
    ;
}



bool Motor::ReadOwnFile()
{
    QFile file(".\\AccConfig\\Motor.xml");

    try
    {
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            //interlock
            qDebug()<<"error:open Motor.xml failed!";
            return false;
        }
        reader.setDevice(&file);
        reader.readNext();

        while (!reader.atEnd())
        {
            if (reader.isStartElement())
            {
                if (reader.name() == "Motor")
                {
                    ReadOwnElement();
                }
                else
                {
                    //interlock
                    qDebug()<<"error: Motor.xml was broken!";
                    return false;
                }
            }
            else
            {
                reader.readNext();
            }
        }
        file.close();
    }
    catch (QString exception)
    {
        //interlock
        return false;
    }
    return true;
}

bool Motor::ReadOwnElement()
{
    for(int i=0;i<12;i++)
        gain_offset_has[i] = false;

    reader.readNext();
    while (!reader.atEnd())
    {
        reader.readNext();
//        if (reader.isEndElement())
//            continue;

        if (reader.isStartElement())
        {
            if (reader.name() == "slow_down_speed")
            {
                while (!reader.isEndElement())
                {
                    if (reader.name() == "jaw_a1")
                        slow_down_speed[JAW_A1] = reader.readElementText().toInt();

                    else if (reader.name() == "jaw_a2")
                        slow_down_speed[JAW_A2] = reader.readElementText().toInt();
                    else if (reader.name() == "jaw_b1")
                        slow_down_speed[JAW_B1] = reader.readElementText().toInt();
                    else if (reader.name() == "jaw_b2")
                        slow_down_speed[JAW_B2] =  reader.readElementText().toInt();
                    else if (reader.name() == "collimator")
                        slow_down_speed[COLLIMATOR] =  reader.readElementText().toInt();
                    else if (reader.name() == "gantry")
                        slow_down_speed[GANTRY] =  reader.readElementText().toInt();

                    reader.readNext();
                }
            }
            else if (reader.name() == "slow_down_distance")
            {
                while (!reader.isEndElement())
                {
                    if (reader.name() == "jaw_a1")
                        slow_down_distance[JAW_A1] = reader.readElementText().toInt();
                    else if (reader.name() == "jaw_a2")
                        slow_down_distance[JAW_A2] = reader.readElementText().toInt();
                    else if (reader.name() == "jaw_b1")
                        slow_down_distance[JAW_B1] = reader.readElementText().toInt();
                    else if (reader.name() == "jaw_b2")
                        slow_down_distance[JAW_B2] =  reader.readElementText().toInt();
                    else if (reader.name() == "collimator")
                        slow_down_distance[COLLIMATOR] =  reader.readElementText().toInt();
                    else if (reader.name() == "gantry")
                        slow_down_distance[GANTRY] =  reader.readElementText().toInt();

                    reader.readNext();
                }
            }
            else if (reader.name() == "max_position_speed")
            {
                while (!reader.isEndElement())
                {
                    if (reader.name() == "jaw_a1")
                        max_position_speed[JAW_A1] = reader.readElementText().toInt();
                    else if (reader.name() == "jaw_a2")
                        max_position_speed[JAW_A2] = reader.readElementText().toInt();
                    else if (reader.name() == "jaw_b1")
                        max_position_speed[JAW_B1] = reader.readElementText().toInt();
                    else if (reader.name() == "jaw_b2")
                        max_position_speed[JAW_B2] =  reader.readElementText().toInt();
                    else if (reader.name() == "collimator")
                        max_position_speed[COLLIMATOR] =  reader.readElementText().toInt();

                    reader.readNext();
                }
            }
            else if (reader.name() == "move_target_speed")
            {
                while (!reader.isEndElement())
                {
                    if (reader.name() == "jaw_a1")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "target_position")
                                move_target_speed[JAW_A1].target_position =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "move_speed")
                                move_target_speed[JAW_A1].move_speed =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "jaw_a2")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "target_position")
                                move_target_speed[JAW_A2].target_position =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "move_speed")
                                move_target_speed[JAW_A2].move_speed =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "jaw_b1")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "target_position")
                                move_target_speed[JAW_B1].target_position =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "move_speed")
                                move_target_speed[JAW_B1].move_speed =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "jaw_b2")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "target_position")
                                move_target_speed[JAW_B2].target_position =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "move_speed")
                                move_target_speed[JAW_B2].move_speed =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "collimator")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "target_position")
                                move_target_speed[COLLIMATOR].target_position =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "move_speed")
                                move_target_speed[COLLIMATOR].move_speed =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "gantry")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "target_position")
                                move_target_speed[GANTRY].target_position =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "move_speed")
                                move_target_speed[GANTRY].move_speed =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    reader.readNext();
                }
            }
            else if (reader.name() == "calibration_speed")
            {
                while (!reader.isEndElement())
                {
                    if (reader.name() == "jaw_a1")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "target_position1")
                                calibration_speed[JAW_A1].target_position1 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "target_position2")
                                calibration_speed[JAW_A1].target_position2 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "speed")
                                calibration_speed[JAW_A1].speed =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "jaw_a2")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "target_position1")
                                calibration_speed[JAW_A2].target_position1 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "target_position2")
                                calibration_speed[JAW_A2].target_position2 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "speed")
                                calibration_speed[JAW_A2].speed =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "jaw_b1")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "target_position1")
                                calibration_speed[JAW_B1].target_position1 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "target_position2")
                                calibration_speed[JAW_B1].target_position2 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "speed")
                                calibration_speed[JAW_B1].speed =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "jaw_b2")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "target_position1")
                                calibration_speed[JAW_B2].target_position1 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "target_position2")
                                calibration_speed[JAW_B2].target_position2 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "speed")
                                calibration_speed[JAW_B2].speed =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }

                    reader.readNext();
                }
            }
            else if (reader.name() == "test_target_speed")
            {
                while (!reader.isEndElement())
                {
                    if (reader.name() == "jaw_a1")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "position1")
                                test_target_speed[JAW_A1].position1 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "position2")
                                test_target_speed[JAW_A1].position2 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "speed")
                                test_target_speed[JAW_A1].speed =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "jaw_a2")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "position1")
                                test_target_speed[JAW_A2].position1 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "position2")
                                test_target_speed[JAW_A2].position2 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "speed")
                                test_target_speed[JAW_A2].speed =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "jaw_b1")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "position1")
                                test_target_speed[JAW_B1].position1 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "position2")
                                test_target_speed[JAW_B1].position2 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "speed")
                                test_target_speed[JAW_B1].speed =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "jaw_b2")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "position1")
                                test_target_speed[JAW_B2].position1 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "position2")
                                test_target_speed[JAW_B2].position2 =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "speed")
                                test_target_speed[JAW_B2].speed =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }

                    reader.readNext();
                }

            }
            else if (reader.name() == "radon_speed_target")
            {
                while (!reader.isEndElement())
                {
                    if (reader.name() == "jaw_a1")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "speed")
                                radon_speed_target[JAW_A1].speed =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "target")
                                radon_speed_target[JAW_A1].target =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "jaw_a2")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "speed")
                                radon_speed_target[JAW_A2].speed =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "target")
                                radon_speed_target[JAW_A2].target =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "jaw_b1")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "speed")
                                radon_speed_target[JAW_B1].speed =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "target")
                                radon_speed_target[JAW_B1].target =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "jaw_b2")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "speed")
                                radon_speed_target[JAW_B2].speed =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "target")
                                radon_speed_target[JAW_B2].target =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "collimator")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "speed")
                                radon_speed_target[COLLIMATOR].speed =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "target")
                                radon_speed_target[COLLIMATOR].target =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    else if (reader.name() == "gantry")
                    {
                        while (!reader.isEndElement())
                        {
                            if (reader.name() == "speed")
                                radon_speed_target[GANTRY].speed =
                                        reader.readElementText().toInt();
                            else if (reader.name() == "target")
                                radon_speed_target[GANTRY].target =
                                        reader.readElementText().toInt();
                            reader.readNext();
                        }
                    }
                    reader.readNext();
                }
            }
            else if (reader.name() == "slow_speed")
            {
                while (!reader.isEndElement())
                {
                    if (reader.name() == "jaw_a1")
                        slow_speed[JAW_A1] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "jaw_a2")
                        slow_speed[JAW_A2] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "jaw_b1")
                        slow_speed[JAW_B1] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "jaw_b2")
                        slow_speed[JAW_B2] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "collimator")
                        slow_speed[COLLIMATOR] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "gantry")
                        slow_speed[GANTRY] =
                                reader.readElementText().toInt();
                    reader.readNext();
                }
            }
            else if (reader.name() == "fast_speed")
            {
                while (!reader.isEndElement())
                {
                    if (reader.name() == "jaw_a1")
                        fast_speed[JAW_A1] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "jaw_a2")
                        fast_speed[JAW_A2] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "jaw_b1")
                        fast_speed[JAW_B1] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "jaw_b2")
                        fast_speed[JAW_B2] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "collimator")
                        fast_speed[COLLIMATOR] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "gantry")
                        fast_speed[GANTRY] =
                                reader.readElementText().toInt();
                    reader.readNext();
                }
            }
            else if (reader.name() == "gain")
            {
                while (!reader.isEndElement())
                {
                    if (reader.name() == "jaw_a1")
                    {
                        gain_offset_has[0] = true;
                        jaw_y1_gain = reader.readElementText().toFloat();
                    }
                    else if (reader.name() == "jaw_a2")
                    {
                        gain_offset_has[1] = true;
                        jaw_y2_gain = reader.readElementText().toFloat();
                    }
                    else if (reader.name() == "jaw_b1")
                    {
                        gain_offset_has[2] = true;
                        jaw_x2_gain = reader.readElementText().toFloat();
                    }
                    else if (reader.name() == "jaw_b2")
                    {
                        gain_offset_has[3] = true;
                        jaw_x1_gain = reader.readElementText().toFloat();
                    }
                    else if (reader.name() == "collimator")
                    {
                        gain_offset_has[4] = true;
                        colli_gain = reader.readElementText().toFloat();
                    }
                    else if (reader.name() == "gantry")
                    {
                        gain_offset_has[5] = true;
                        gantry_gain = reader.readElementText().toFloat();
                    }
                    reader.readNext();
                }
            }
            else if (reader.name() == "offset")
            {
                while (!reader.isEndElement())
                {
                    if (reader.name() == "jaw_a1")
                    {
                        gain_offset_has[6] = true;
                        jaw_y1_offset = reader.readElementText().toFloat();
                    }
                    else if (reader.name() == "jaw_a2")
                    {
                        gain_offset_has[7] = true;
                        jaw_y2_offset = reader.readElementText().toFloat();
                    }
                    else if (reader.name() == "jaw_b1")
                    {
                        gain_offset_has[8] = true;
                        jaw_x2_offset = reader.readElementText().toFloat();
                    }
                    else if (reader.name() == "jaw_b2")
                    {
                        gain_offset_has[9] = true;
                        jaw_x1_offset = reader.readElementText().toFloat();
                    }
                    else if (reader.name() == "collimator")
                    {
                        gain_offset_has[10] = true;
                        colli_offset = reader.readElementText().toFloat();
                    }
                    else if (reader.name() == "gantry")
                    {
                        gain_offset_has[11] = true;
                        gantry_offset = reader.readElementText().toFloat();
                    }
                    reader.readNext();
                }
            }
            else if (reader.name() == "colli_type")
            {
                colli_type = (COLLI_TYPE_T)reader.readElementText().toInt();
            }
            else if (reader.name() == "error_mask_1")
            {
                while (!reader.isEndElement())
                {
                    if (reader.name() == "byte_0")
                        error_mask_1.error_mask_bytes[0] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "byte_1")
                        error_mask_1.error_mask_bytes[1] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "byte_2")
                        error_mask_1.error_mask_bytes[2] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "byte_3")
                        error_mask_1.error_mask_bytes[3] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "byte_4")
                        error_mask_1.error_mask_bytes[4] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "byte_5")
                        error_mask_1.error_mask_bytes[5] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "byte_6")
                        error_mask_1.error_mask_bytes[6] =
                                reader.readElementText().toInt();
                    reader.readNext();
                }
            }
            else if (reader.name() == "error_mask_2")
            {
                while (!reader.isEndElement())
                {
                    if (reader.name() == "byte_0")
                        error_mask_2.error_mask_bytes[0] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "byte_1")
                        error_mask_2.error_mask_bytes[1] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "byte_2")
                        error_mask_2.error_mask_bytes[2] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "byte_3")
                        error_mask_2.error_mask_bytes[3] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "byte_4")
                        error_mask_2.error_mask_bytes[4] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "byte_5")
                        error_mask_2.error_mask_bytes[5] =
                                reader.readElementText().toInt();
                    else if (reader.name() == "byte_6")
                        error_mask_2.error_mask_bytes[6] =
                                reader.readElementText().toInt();
                    reader.readNext();
                }
            }
            else if (reader.name() == "gantry_speed_factor_cw")
            {
                gantry_speed_factor_cw = reader.readElementText().toFloat();
            }
            else if (reader.name() == "gantry_speed_factor_ccw")
            {
                gantry_speed_factor_ccw = reader.readElementText().toFloat();
            }
        }

    }
//    if(!CheckMotorSpeedData())
//        InsertInterlock(62, 885, ERROR_IL, IL_ALL, LATCHED);
    if(!CheckGantrySpeedFactor())
        InsertInterlock(62, 883, ERROR_IL, IL_ALL, LATCHED);
    if(!CheckCalibDataIntegrity())
        InsertInterlock(62, 806, ERROR_IL, IL_ALL, LATCHED);
    return true;
}

bool Motor::WriteOwnFile()
{
    QFile file(".\\AccConfig\\Motor.xml");

    try
    {
        if (!file.open(QFile::ReadWrite | QFile::Text))
        {
            //interlock
            qDebug()<<"error: open Motor.xml failed!";
            return false;
        }
        if (!doc.setContent(&file))
        {
            //interlock
            file.close();
            return false;
        }
        file.close();
        root = doc.documentElement();
        while (!root.isNull())
        {
            if (root.tagName() == "Motor")
            {
                WriteOwnElement();
                file.open(QFile::WriteOnly | QFile::Truncate);
                QTextStream ts(&file);
                ts.reset();
                ts.setCodec("utf-8");
                doc.save(ts, 4, QDomNode::EncodingFromTextStream);

            }
            root = root.nextSibling().toElement();
        }
        file.close();
        return true;

    }
    catch (QString exception)
    {
        //interlock
        return false;
    }
    return true;
}

bool Motor::WriteOwnElement()
{
    QDomNode  node_l1, node_l2;
    QDomNodeList list_l1, list_l2;

    node = root.firstChild();
    while (!node.isNull())
    {
        if (node.nodeName() == "slow_down_speed")
        {
            list_l1 = node.childNodes();
            for (int i = 0; i < list_l1.count(); i++)
            {
                node_l1 = list_l1.at(i);
                if (node_l1.nodeName() == "jaw_a1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_down_speed[JAW_A1]));
                else if (node_l1.nodeName() == "jaw_a2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_down_speed[JAW_A2]));
                else if (node_l1.nodeName() == "jaw_b1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_down_speed[JAW_B1]));
                else if (node_l1.nodeName() == "jaw_b2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_down_speed[JAW_B2]));
                else if (node_l1.nodeName() == "collimator")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_down_speed[COLLIMATOR]));
                else if (node_l1.nodeName() == "gantry")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_down_speed[GANTRY]));
            }
        }
        else if (node.nodeName() == "slow_down_distance")
        {
            list_l1 = node.childNodes();
            for (int i = 0; i < list_l1.count(); i++)
            {
                node_l1 = list_l1.at(i);
                if (node_l1.nodeName() == "jaw_a1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_down_distance[JAW_A1]));
                else if (node_l1.nodeName() == "jaw_a2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_down_distance[JAW_A2]));
                else if (node_l1.nodeName() == "jaw_b1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_down_distance[JAW_B1]));
                else if (node_l1.nodeName() == "jaw_b2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_down_distance[JAW_B2]));
                else if (node_l1.nodeName() == "collimator")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_down_distance[COLLIMATOR]));
                else if (node_l1.nodeName() == "gantry")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_down_distance[GANTRY]));
            }
        }
        else if (node.nodeName() == "max_position_speed")
        {
            list_l1 = node.childNodes();
            for (int i = 0; i < list_l1.count(); i++)
            {
                node_l1 = list_l1.at(i);
                if (node_l1.nodeName() == "jaw_a1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(max_position_speed[JAW_A1]));
                else if (node_l1.nodeName() == "jaw_a2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(max_position_speed[JAW_A2]));
                else if (node_l1.nodeName() == "jaw_b1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(max_position_speed[JAW_B1]));
                else if (node_l1.nodeName() == "jaw_b2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(max_position_speed[JAW_B2]));
                else if (node_l1.nodeName() == "collimator")
                    node_l1.firstChild().setNodeValue(
                                QString::number(max_position_speed[COLLIMATOR]));
            }
        }
        else if (node.nodeName() == "calibration_speed")
        {
            list_l1 = node.childNodes();
            for (int i = 0; i < list_l1.count(); i++)
            {
                node_l1 = list_l1.at(i);
                if (node_l1.nodeName() == "jaw_a1")
                {
                    list_l2 = node_l1.childNodes();
                    for (int j = 0; j < list_l2.count(); j++)
                    {
                        node_l2 = list_l2.at(j);
                        if (node_l2.nodeName() == "target_position1")
                            node_l1.firstChild().setNodeValue(
                                        QString::number(calibration_speed[JAW_A1].target_position1));
                        else if (node_l2.nodeName() == "target_position2")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(calibration_speed[JAW_A1].target_position1));
                        else if (node_l2.nodeName() == "speed")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(calibration_speed[JAW_A1].speed));
                    }
                }
                else if (node_l1.nodeName() == "jaw_a2")
                {
                    list_l2 = node_l1.childNodes();
                    for (int j = 0; j < list_l2.count(); j++)
                    {
                        node_l2 = list_l2.at(j);
                        if (node_l2.nodeName() == "target_position1")
                            node_l1.firstChild().setNodeValue(
                                        QString::number(calibration_speed[JAW_A2].target_position1));
                        else if (node_l2.nodeName() == "target_position2")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(calibration_speed[JAW_A2].target_position1));
                        else if (node_l2.nodeName() == "speed")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(calibration_speed[JAW_A2].speed));
                    }
                }
                else if (node_l1.nodeName() == "jaw_b1")
                {
                    list_l2 = node_l1.childNodes();
                    for (int j = 0; j < list_l2.count(); j++)
                    {
                        node_l2 = list_l2.at(j);
                        if (node_l2.nodeName() == "target_position1")
                            node_l1.firstChild().setNodeValue(
                                        QString::number(calibration_speed[JAW_B1].target_position1));
                        else if (node_l2.nodeName() == "target_position2")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(calibration_speed[JAW_B1].target_position1));
                        else if (node_l2.nodeName() == "speed")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(calibration_speed[JAW_B1].speed));
                    }
                }
                else if (node_l1.nodeName() == "jaw_b2")
                {
                    list_l2 = node_l1.childNodes();
                    for (int j = 0; j < list_l2.count(); j++)
                    {
                        node_l2 = list_l2.at(j);
                        if (node_l2.nodeName() == "target_position1")
                            node_l1.firstChild().setNodeValue(
                                        QString::number(calibration_speed[JAW_B2].target_position1));
                        else if (node_l2.nodeName() == "target_position2")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(calibration_speed[JAW_B2].target_position1));
                        else if (node_l2.nodeName() == "speed")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(calibration_speed[JAW_B2].speed));
                    }
                }
            }
        }
        else if (node.nodeName() == "test_target_speed")
        {
            list_l1 = node.childNodes();
            for (int i = 0; i < list_l1.count(); i++)
            {
                node_l1 = list_l1.at(i);
                if (node_l1.nodeName() == "jaw_a1")
                {
                    list_l2 = node_l1.childNodes();
                    for (int j = 0; j < list_l2.count(); j++)
                    {
                        node_l2 = list_l2.at(j);
                        if (node_l2.nodeName() == "position1")
                            node_l1.firstChild().setNodeValue(
                                        QString::number(test_target_speed[JAW_A1].position1));
                        else if (node_l2.nodeName() == "position2")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(test_target_speed[JAW_A1].position2));
                        else if (node_l2.nodeName() == "speed")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(test_target_speed[JAW_A1].speed));
                    }
                }
                else if (node_l1.nodeName() == "jaw_a2")
                {
                    list_l2 = node_l1.childNodes();
                    for (int j = 0; j < list_l2.count(); j++)
                    {
                        node_l2 = list_l2.at(j);
                        if (node_l2.nodeName() == "position1")
                            node_l1.firstChild().setNodeValue(
                                        QString::number(test_target_speed[JAW_A2].position1));
                        else if (node_l2.nodeName() == "position2")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(test_target_speed[JAW_A2].position2));
                        else if (node_l2.nodeName() == "speed")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(test_target_speed[JAW_A2].speed));
                    }
                }
                else if (node_l1.nodeName() == "jaw_b1")
                {
                    list_l2 = node_l1.childNodes();
                    for (int j = 0; j < list_l2.count(); j++)
                    {
                        node_l2 = list_l2.at(j);
                        if (node_l2.nodeName() == "position1")
                            node_l1.firstChild().setNodeValue(
                                        QString::number(test_target_speed[JAW_B1].position1));
                        else if (node_l2.nodeName() == "position2")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(test_target_speed[JAW_B1].position2));
                        else if (node_l2.nodeName() == "speed")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(test_target_speed[JAW_B1].speed));
                    }
                }
                else if (node_l1.nodeName() == "jaw_b2")
                {
                    list_l2 = node_l1.childNodes();
                    for (int j = 0; j < list_l2.count(); j++)
                    {
                        node_l2 = list_l2.at(j);
                        if (node_l2.nodeName() == "position1")
                            node_l1.firstChild().setNodeValue(
                                        QString::number(test_target_speed[JAW_B2].position1));
                        else if (node_l2.nodeName() == "position2")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(test_target_speed[JAW_B2].position2));
                        else if (node_l2.nodeName() == "speed")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(test_target_speed[JAW_B2].speed));
                    }
                }
            }
        }
        else if (node.nodeName() == "radon_speed_target")
        {
            list_l1 = node.childNodes();
            for (int i = 0; i < list_l1.count(); i++)
            {
                node_l1 = list_l1.at(i);
                if (node_l1.nodeName() == "jaw_a1")
                {
                    list_l2 = node_l1.childNodes();
                    for (int j = 0; j < list_l2.count(); j++)
                    {
                        node_l2 = list_l2.at(j);
                        if (node_l2.nodeName() == "speed")
                            node_l1.firstChild().setNodeValue(
                                        QString::number(radon_speed_target[JAW_A1].speed));
                        else if (node_l2.nodeName() == "target")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(radon_speed_target[JAW_A1].target));
                    }
                }
                else if (node_l1.nodeName() == "jaw_a2")
                {
                    list_l2 = node_l1.childNodes();
                    for (int j = 0; j < list_l2.count(); j++)
                    {
                        node_l2 = list_l2.at(j);
                        if (node_l2.nodeName() == "speed")
                            node_l1.firstChild().setNodeValue(
                                        QString::number(radon_speed_target[JAW_A2].speed));
                        else if (node_l2.nodeName() == "target")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(radon_speed_target[JAW_A2].target));
                    }
                }
                else if (node_l1.nodeName() == "jaw_b1")
                {
                    list_l2 = node_l1.childNodes();
                    for (int j = 0; j < list_l2.count(); j++)
                    {
                        node_l2 = list_l2.at(j);
                        if (node_l2.nodeName() == "speed")
                            node_l1.firstChild().setNodeValue(
                                        QString::number(radon_speed_target[JAW_B1].speed));
                        else if (node_l2.nodeName() == "target")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(radon_speed_target[JAW_B1].target));
                    }
                }
                else if (node_l1.nodeName() == "jaw_b2")
                {
                    list_l2 = node_l1.childNodes();
                    for (int j = 0; j < list_l2.count(); j++)
                    {
                        node_l2 = list_l2.at(j);
                        if (node_l2.nodeName() == "speed")
                            node_l1.firstChild().setNodeValue(
                                        QString::number(radon_speed_target[JAW_B2].speed));
                        else if (node_l2.nodeName() == "target")
                            node_l2.firstChild().setNodeValue(
                                        QString::number(radon_speed_target[JAW_B2].target));
                    }
                }
            }

        }
        else if (node.nodeName() == "slow_speed")
        {
            list_l1 = node.childNodes();
            for (int i = 0; i < list_l1.count(); i++)
            {
                node_l1 = list_l1.at(i);
                if (node_l1.nodeName() == "jaw_a1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_speed[JAW_A1]));
                else if (node_l1.nodeName() == "jaw_a2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_speed[JAW_A2]));
                else if (node_l1.nodeName() == "jaw_b1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_speed[JAW_B1]));
                else if (node_l1.nodeName() == "jaw_b2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_speed[JAW_B2]));
                else if (node_l1.nodeName() == "collimator")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_speed[COLLIMATOR]));
                else if (node_l1.nodeName() == "gantry")
                    node_l1.firstChild().setNodeValue(
                                QString::number(slow_speed[GANTRY]));
            }
        }
        else if (node.nodeName() == "fast_speed")
        {
            list_l1 = node.childNodes();
            for (int i = 0; i < list_l1.count(); i++)
            {
                node_l1 = list_l1.at(i);
                if (node_l1.nodeName() == "jaw_a1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(fast_speed[JAW_A1]));
                else if (node_l1.nodeName() == "jaw_a2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(fast_speed[JAW_A2]));
                else if (node_l1.nodeName() == "jaw_b1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(fast_speed[JAW_B1]));
                else if (node_l1.nodeName() == "jaw_b2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(fast_speed[JAW_B2]));
                else if (node_l1.nodeName() == "collimator")
                    node_l1.firstChild().setNodeValue(
                                QString::number(fast_speed[COLLIMATOR]));
                else if (node_l1.nodeName() == "gantry")
                    node_l1.firstChild().setNodeValue(
                                QString::number(fast_speed[GANTRY]));
            }
        }
        else if (node.nodeName() == "gain")
        {
            list_l1 = node.childNodes();
            for (int i = 0; i < list_l1.count(); i++)
            {
                node_l1 = list_l1.at(i);
                if (node_l1.nodeName() == "jaw_a1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(jaw_y1_gain));
                else if (node_l1.nodeName() == "jaw_a2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(jaw_y2_gain));
                else if (node_l1.nodeName() == "jaw_b1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(jaw_x2_gain));
                else if (node_l1.nodeName() == "jaw_b2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(jaw_x1_gain));
                else if (node_l1.nodeName() == "collimator")
                    node_l1.firstChild().setNodeValue(
                                QString::number(colli_gain));
                else if (node_l1.nodeName() == "gantry")
                    node_l1.firstChild().setNodeValue(
                                QString::number(gantry_gain));
            }
        }
        else if (node.nodeName() == "offset")
        {
            list_l1 = node.childNodes();
            for (int i = 0; i < list_l1.count(); i++)
            {
                node_l1 = list_l1.at(i);
                if (node_l1.nodeName() == "jaw_a1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(jaw_y1_offset));
                else if (node_l1.nodeName() == "jaw_a2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(jaw_y2_offset));
                else if (node_l1.nodeName() == "jaw_b1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(jaw_x2_offset));
                else if (node_l1.nodeName() == "jaw_b2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(jaw_x1_offset));
                else if (node_l1.nodeName() == "collimator")
                    node_l1.firstChild().setNodeValue(
                                QString::number(colli_offset));
                else if (node_l1.nodeName() == "gantry")
                    node_l1.firstChild().setNodeValue(
                                QString::number(gantry_offset));
            }
        }
        else if (node.nodeName() == "colli_type")
            node.firstChild().setNodeValue(
                        QString::number(colli_type));
        else if (node.nodeName() == "error_mask_1")
        {
            list_l1 = node.childNodes();
            for (int i = 0; i < list_l1.count(); i++)
            {
                node_l1 = list_l1.at(i);
                if (node_l1.nodeName() == "byte_0")
                    node_l1.firstChild().setNodeValue(
                                QString::number(error_mask_1.error_mask_bytes[0]));
                else if (node_l1.nodeName() == "byte_1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(error_mask_1.error_mask_bytes[1]));
                else if (node_l1.nodeName() == "byte_2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(error_mask_1.error_mask_bytes[2]));
                else if (node_l1.nodeName() == "byte_3")
                    node_l1.firstChild().setNodeValue(
                                QString::number(error_mask_1.error_mask_bytes[3]));
                else if (node_l1.nodeName() == "byte_4")
                    node_l1.firstChild().setNodeValue(
                                QString::number(error_mask_1.error_mask_bytes[4]));
                else if (node_l1.nodeName() == "byte_5")
                    node_l1.firstChild().setNodeValue(
                                QString::number(error_mask_1.error_mask_bytes[5]));
                else if (node_l1.nodeName() == "byte_6")
                    node_l1.firstChild().setNodeValue(
                                QString::number(error_mask_1.error_mask_bytes[6]));
            }
        }
        else if (node.nodeName() == "error_mask_2")
        {
            list_l1 = node.childNodes();
            for (int i = 0; i < list_l1.count(); i++)
            {
                node_l1 = list_l1.at(i);
                if (node_l1.nodeName() == "byte_0")
                    node_l1.firstChild().setNodeValue(
                                QString::number(error_mask_2.error_mask_bytes[0]));
                else if (node_l1.nodeName() == "byte_1")
                    node_l1.firstChild().setNodeValue(
                                QString::number(error_mask_2.error_mask_bytes[1]));
                else if (node_l1.nodeName() == "byte_2")
                    node_l1.firstChild().setNodeValue(
                                QString::number(error_mask_2.error_mask_bytes[2]));
                else if (node_l1.nodeName() == "byte_3")
                    node_l1.firstChild().setNodeValue(
                                QString::number(error_mask_2.error_mask_bytes[3]));
                else if (node_l1.nodeName() == "byte_4")
                    node_l1.firstChild().setNodeValue(
                                QString::number(error_mask_2.error_mask_bytes[4]));
                else if (node_l1.nodeName() == "byte_5")
                    node_l1.firstChild().setNodeValue(
                                QString::number(error_mask_2.error_mask_bytes[5]));
                else if (node_l1.nodeName() == "byte_6")
                    node_l1.firstChild().setNodeValue(
                                QString::number(error_mask_2.error_mask_bytes[6]));
            }
        }
        else if (node.nodeName() == "gantry_speed_factor_cw")
        {
            node.firstChild().setNodeValue(QString::number(gantry_speed_factor_cw));
        }
        else if (node.nodeName() == "gantry_speed_factor_ccw")
        {
            node.firstChild().setNodeValue(QString::number(gantry_speed_factor_ccw));
        }
        node = node.nextSibling();
    }

    return true;
}

bool Motor::CheckCalibDataIntegrity()
{
    for(int i=0;i<12;i++)
    {
        if(!gain_offset_has[i])
        {
            qDebug()<<"Motor calibrate integrity error:  i ="<<i;
            return false;
        }
    }
    return true;
}

bool Motor::CheckMotorSpeedData()
{
    bool suc =true;

    suc &= CheckValue(slow_speed[JAW_A1],JAWY1_SS_RSCH_CK);
    suc &= CheckValue(slow_speed[JAW_A2],JAWY2_SS_RSCH_CK);
    suc &= CheckValue(slow_speed[JAW_B1],JAWX2_SS_RSCH_CK);
    suc &= CheckValue(slow_speed[JAW_B2],JAWX1_SS_RSCH_CK);
    suc &= CheckValue(slow_speed[COLLIMATOR],COLLI_SS_RSCH_CK);
    suc &= CheckValue(slow_speed[GANTRY],GANTRY_SS_RSCH_CK);

    suc &= CheckValue(fast_speed[JAW_A1],JAWY1_FS_RSCH_CK);
    suc &= CheckValue(fast_speed[JAW_A2],JAWY2_FS_RSCH_CK);
    suc &= CheckValue(fast_speed[JAW_B1],JAWX2_FS_RSCH_CK);
    suc &= CheckValue(fast_speed[JAW_B2],JAWX1_FS_RSCH_CK);
    suc &= CheckValue(fast_speed[COLLIMATOR],COLLI_FS_RSCH_CK);
    suc &= CheckValue(fast_speed[GANTRY],GANTRY_FS_RSCH_CK);

    suc &= CheckValue(slow_down_speed[JAW_A1],JAWY1_SDS_RSCH_CK);
    suc &= CheckValue(slow_down_speed[JAW_A2],JAWY2_SDS_RSCH_CK);
    suc &= CheckValue(slow_down_speed[JAW_B1],JAWX2_SDS_RSCH_CK);
    suc &= CheckValue(slow_down_speed[JAW_B2],JAWX1_SDS_RSCH_CK);
    suc &= CheckValue(slow_down_speed[COLLIMATOR],COLLI_SDS_RSCH_CK);
    suc &= CheckValue(slow_down_speed[GANTRY],GANTRY_SDS_RSCH_CK);

    suc &= CheckValue(slow_down_distance[JAW_A1],JAWY1_SDD_RSCH_CK);
    suc &= CheckValue(slow_down_distance[JAW_A2],JAWY2_SDD_RSCH_CK);
    suc &= CheckValue(slow_down_distance[JAW_B1],JAWX2_SDD_RSCH_CK);
    suc &= CheckValue(slow_down_distance[JAW_B2],JAWX1_SDD_RSCH_CK);
    suc &= CheckValue(slow_down_distance[COLLIMATOR],COLLI_SDD_RSCH_CK);
    suc &= CheckValue(slow_down_distance[GANTRY],GANTRY_SDD_RSCH_CK);

    return suc;
}

bool Motor::CheckGantrySpeedFactor()
{
    bool suc;
    suc = CheckValue(gantry_speed_factor_cw,GANTRY_SPEED_FACTOR_CW_CK);
    suc &= CheckValue(gantry_speed_factor_ccw,GANTRY_SPEED_FACTOR_CCW_CK);
    return suc;
}


//计算倒数第2位校验和，高4位表示序号，低4位异或得到
void Motor::ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc)
{
    UINT8 low = 0, tmplow = 0;

    fc = send_frame_num << 4;

    send_frame_num = (send_frame_num + 1) % 16;

    low = data[0] ^ len;
    for (int i = 1; i < len; i++)
        low = low ^ data[i];

    tmplow = (low & 0x0F) ^ ((low & 0xF0) >> 4);
    fc += tmplow;
}

UINT8 Motor::ComputeRevCHK(UINT8* data, UINT8 len)
{
    UINT8 fc;
    UINT8 low = 0, tmplow = 0;

    // rev_frame_num_mutex.lock();
    fc = rev_frame_num << 4;
    rev_frame_num = (rev_frame_num + 1) % 16;
    // rev_frame_num_mutex.unlock();

    low = data[0] ^ len;
    for (int i = 1; i < len; i++)
        low = low ^ data[i];

    tmplow = (low & 0x0F) ^ ((low & 0xF0) >> 4);
    fc += tmplow;

    return fc;
}

bool Motor::SendC0()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = 0x20;
    tmpSend.data[1] = 0xc0;
    tmpSend.data[2] = 0x00;
    tmpSend.data[3] = 0xe0;
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool Motor::SendSlowDownSpeed()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x41;         //A

    //使用数组方式定义数据结构
    for (int i = 0; i < 6; i++)
        tmpSend.data[i + 2] = slow_down_speed[i];

    // 使用锁防止FC乱序
    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&tmpSend.data[0], 8, tmpSend.data[8]);
    tmpSend.data[9] = ComputeLstCHK(&tmpSend.data[0], 9);
    tmpSend.len = 10;       //fixme

    return InsertOutQueue(tmpSend);
}

bool Motor::SendSlowDownDistance()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x42;         //B

    for (int i = 0; i < 6; i++)
        tmpSend.data[i + 2] = slow_down_distance[i];

    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&tmpSend.data[0], 8, tmpSend.data[8]);
    tmpSend.data[9] = ComputeLstCHK(&tmpSend.data[0], 9);
    tmpSend.len = 10;

    return InsertOutQueue(tmpSend);
}

bool Motor::SendUserMode(const USERMODE_TYPE_T mode)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    if (mode > SERVICE_MODE || mode < RESET_MODE)
        return false;
    mode_now = mode;

    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x43;     //C
    tmpSend.data[2] = mode;

    QMutexLocker locker(&motor_mutex);
    if (mode_now == RESET_MODE)
    {
        send_frame_num = 0;         //发送reset命令，将序列号清零
      //  rev_frame_num = 0;
    }

    ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]);
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool Motor::SendAccept()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x49;     //I

    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]);
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

bool Motor::SendErrorMask()
{
    bool res;
    TransRecvT errmask1, errmask2;
    memset(&errmask1, 0, sizeof (errmask1));
    memset(&errmask2, 0, sizeof (errmask2));

    errmask1.data[0] = TOMOTOR;
    errmask1.data[1] = 0x4D;        //M
    errmask1.data[2] = ERR_MASK1;

    errmask2.data[0] = TOMOTOR;
    errmask2.data[1] = 0x4D;
    errmask2.data[2] = ERR_MASK2;

    for (int i = 0; i < 7; i++)
        errmask1.data[i + 3] = error_mask_1.error_mask_bytes[i];
    for (int i = 0; i < 7; i++)
        errmask2.data[i + 3] = error_mask_2.error_mask_bytes[i];

    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&errmask1.data[0], 10, errmask1.data[10]);
    errmask1.data[11] = ComputeLstCHK(&errmask1.data[0], 11);
    errmask1.len = 12;

    res = InsertOutQueue(errmask1);

    ComputeSendCHK(&errmask2.data[0], 10, errmask2.data[10]);
    errmask2.data[11] = ComputeLstCHK(&errmask2.data[0], 11);
    errmask2.len = 12;

    res &= InsertOutQueue(errmask2);
    return res;
}

bool Motor::SendInterrupt()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x4F;

    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]);
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

bool Motor::SendMoveNow(const MOTOR_PARTS_T part, const UINT16 target)
{
    TransRecvT tmpSend;

    if (part < JAW_A1 || part > GANTRY)
        return false;

    move_target_speed[part].target_position = target;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x50;     //P
    tmpSend.data[2] = part;
    tmpSend.data[3] = (UINT8)(target & 0x00FF);
    tmpSend.data[4] = (UINT8)((target  >> 8) & 0x00FF);
    tmpSend.data[5] = (UINT8)(fast_speed[part] & 0x00FF);
    tmpSend.data[6] = (UINT8)((fast_speed[part] >> 8) & 0x00FF);

    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&tmpSend.data[0], 7, tmpSend.data[7]);
    tmpSend.data[8] = ComputeLstCHK(&tmpSend.data[0], 8);
    tmpSend.len = 9;

    return InsertOutQueue(tmpSend);
}

bool Motor::SendMoveNow(const MOTOR_PARTS_T part)
{
    TransRecvT tmpSend;

    if (part < JAW_A1 || part > GANTRY)
        return false;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x50;     //P
    tmpSend.data[2] = part;
    tmpSend.data[3] = (UINT8)(move_target_speed[part].target_position & 0x00FF);
    tmpSend.data[4] = (UINT8)((move_target_speed[part].target_position  >> 8) & 0x00FF);

    tmpSend.data[5] = (UINT8)(fast_speed[part] & 0x00FF);
    tmpSend.data[6] = (UINT8)((fast_speed[part] >> 8) & 0x00FF);

    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&tmpSend.data[0], 7, tmpSend.data[7]);
    tmpSend.data[8] = ComputeLstCHK(&tmpSend.data[0], 8);
    tmpSend.len = 9;

    return InsertOutQueue(tmpSend);
}

bool Motor::SendMoveNow(const MOTOR_PARTS_T part, const UINT16 target, UINT8 speed)
{
    //11-26 only used in hand
    TransRecvT tmpSend;

    if (part < JAW_A1 || part > GANTRY)
        return false;

    //LJY-12-13
    //手动控制不记录target_position，以免运动中突然停止，使target_position与position[]不匹配
    //move_target_speed[part].target_position = target;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x50;     //P
    tmpSend.data[2] = part;
    tmpSend.data[3] = (UINT8)(target & 0x00FF);
    tmpSend.data[4] = (UINT8)((target  >> 8) & 0x00FF);

    if(speed == 0x2d)
    {
        tmpSend.data[5] = (UINT8)(slow_speed[part] & 0x00FF);
        tmpSend.data[6] = (UINT8)((slow_speed[part] >> 8) & 0x00FF);
    }
    else
    {
        //default use fast speed
        tmpSend.data[5] = (UINT8)(fast_speed[part] & 0x00FF);
        tmpSend.data[6] = (UINT8)((fast_speed[part] >> 8) & 0x00FF);
    }

    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&tmpSend.data[0], 7, tmpSend.data[7]);
    tmpSend.data[8] = ComputeLstCHK(&tmpSend.data[0], 8);
    tmpSend.len = 9;

    return InsertOutQueue(tmpSend);
}

//not uesd
bool Motor::SendSpeedCalibration(const MOTOR_PARTS_T part, const UINT16 target_position1,
                                 const UINT16 target_position2, const UINT16 speed)
{

    TransRecvT tmpSend;

    if (part < JAW_A1 || part > JAW_B2)
        return false;

    calibration_speed[part].target_position1 = target_position1;
    calibration_speed[part].target_position2 = target_position2;
    calibration_speed[part].speed = speed;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x51;     //Q
    tmpSend.data[2] = part;
    tmpSend.data[3] = (UINT8)(target_position1 & 0x00FF);
    tmpSend.data[4] = (UINT8)((target_position1 >> 8) & 0x00FF);
    tmpSend.data[5] = (UINT8)(target_position2 & 0x00FF);
    tmpSend.data[6] = (UINT8)((target_position2 >> 8) & 0x00FF);
    tmpSend.data[7] = (UINT8)(speed & 0x00FF);
    tmpSend.data[8] = (UINT8)((speed >> 8) & 0x00FF);

    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&tmpSend.data[0], 9, tmpSend.data[9]);
    tmpSend.data[10] = ComputeLstCHK(&tmpSend.data[0], 10);
    tmpSend.len = 11;

    return InsertOutQueue(tmpSend);
}

//bool Motor::SendMoveInRadOn(const MOTOR_PARTS_T part, const UINT16 position)
//LJY-9-23
bool Motor::SendMoveInRadOn()
{
    TransRecvT tmpSend;

    //radon_speed_target[part].target = position;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x52;     //R
    tmpSend.data[2] = 0x05;
    tmpSend.data[3] = (UINT8)((UINT16)rot_stop_position & 0x00FF);
    tmpSend.data[4] = (UINT8)(((UINT16)rot_stop_position & 0xFF00) >> 8);

    //LJY-9-23
    tmpSend.data[5] = (UINT8)(dosei->rot_speed & 0x00FF);
    tmpSend.data[6] = (UINT8)((dosei->rot_speed & 0xFF00) >> 8);
    //tmpSend.data[7] = (UINT8)is_auto_sequenced;

    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&tmpSend.data[0], 7, tmpSend.data[7]);
    tmpSend.data[8] = ComputeLstCHK(&tmpSend.data[0], 8);
    tmpSend.len = 9;

    return InsertOutQueue(tmpSend);
}

//not uesd
bool Motor::SendSpeedTest(const MOTOR_PARTS_T part, const UINT16 target_position1,
                          const UINT16 target_position2, const UINT16 speed)
{
    TransRecvT tmpSend;

    if (part < JAW_A1 || part > GANTRY)
        return false;

    if (test_target_speed[part].position1 == target_position1 &&
            test_target_speed[part].position2 == target_position2 &&
            test_target_speed[part].speed == speed)
        return false;

    test_target_speed[part].position1 = target_position1;
    test_target_speed[part].position2 = target_position2;
    test_target_speed[part].speed = speed;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x53;     //S
    tmpSend.data[2] = part;
    tmpSend.data[3] = (UINT8)(target_position1 & 0x00FF);
    tmpSend.data[4] = (UINT8)((target_position1 & 0xFF00) >> 8);
    tmpSend.data[5] = (UINT8)(target_position2 & 0x00FF);
    tmpSend.data[6] = (UINT8)((target_position2 & 0xFF00) >> 8);
    tmpSend.data[7] = (UINT8)(speed & 0x00FF);
    tmpSend.data[8] = (UINT8)((speed & 0xFF00) >> 8);

    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&tmpSend.data[0], 9, tmpSend.data[9]);
    tmpSend.data[10] = ComputeLstCHK(&tmpSend.data[0], 10);
    tmpSend.len = 11;

    return InsertOutQueue(tmpSend);
}

//not used
bool Motor::SendMaxJawSpeedColliType(const UINT16 speed, const MOTOR_PARTS_T part)
{
    TransRecvT tmpSend;
    if (part < JAW_A1 || part > COLLIMATOR)
        return false;
    if ( max_position_speed[part] == speed)
        return false;
    max_position_speed[part] = part;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x56;     //V

    for (int i = 0; i < 4; i++)
    {
        tmpSend.data[i * 2 + 2] = (UINT8)(max_position_speed[i] & 0x00FF);
        tmpSend.data[i * 2 + 3] = (UINT8)((max_position_speed[i] & 0xFF00) >> 8);
    }

    tmpSend.data[10] = (UINT8)(max_position_speed[4] & 0x00FF);


    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&tmpSend.data[0], 11, tmpSend.data[11]);

    tmpSend.data[12] = ComputeLstCHK(&tmpSend.data[0], 12);
    tmpSend.len = 13;

    return InsertOutQueue(tmpSend);
}

bool Motor::SendMaxJawSpeedColliType()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x56;     //V

    for (int i = 0; i < 4; i++)
    {
        tmpSend.data[i * 2 + 2] = (UINT8)(max_position_speed[i] & 0x00FF);
        tmpSend.data[i * 2 + 3] = (UINT8)((max_position_speed[i] & 0xFF00) >> 8);
    }

    tmpSend.data[10] = (UINT8)(max_position_speed[4] & 0x00FF);

    ComputeSendCHK(&tmpSend.data[0], 11, tmpSend.data[11]);

    QMutexLocker locker(&motor_mutex);
    tmpSend.data[12] = ComputeLstCHK(&tmpSend.data[0], 12);
    tmpSend.len = 13;

    return InsertOutQueue(tmpSend);
}

bool Motor::SendCheckRequest(const CHECKREQ_TYPE_T index)
{
    TransRecvT tmpSend;

    if (index != CHECK1 && index != CHECK2 &&
            index != CHECK2_WATCHDOG)
        return false;

    check_index = index;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x57;     //W
    tmpSend.data[2] = index;

    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]);

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;
    return InsertOutQueue(tmpSend);
}

bool Motor::SendStop(const MOTOR_PARTS_T part)
{
    TransRecvT tmpSend;

    if (part < JAW_A1 || (part > GANTRY && part != ALL_MOTORS))
        return false;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x58;     //X
    tmpSend.data[2] = part;

    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]);

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;
    return InsertOutQueue(tmpSend);
}

bool Motor::SendReadyStatus(const READYSTATE_TYPE_T status)
{
    TransRecvT tmpSend;

    if (status != NOTREADY && status != READY && status != INTERRUPTREDY
            && status != PAUSE && status != INTERRUPTNOTREADY)
        return false;

    ready_status_now = status;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOMOTOR;
    tmpSend.data[1] = 0x5A;     //Z
    tmpSend.data[2] = status;

    QMutexLocker locker(&motor_mutex);
    ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]);

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

void Motor::CheckMotorSetActPos()
{
    UINT16 tmp_x1 = 0;
    UINT16 tmp_x2 = 0;
    UINT16 tmp_y1 = 0;
    UINT16 tmp_y2 = 0;
    UINT16 tmp_colli = 0;
    UINT16 tmp_gantry = 0;

    if(io->acc2_exist)
    {
        //qDebug()<<"acc2 ="<<io->accessory_2_type;
        switch (io->accessory_2_type)
        {
        case RW_15:
        case RW_30:
        case RW_45:
            //field size: x*y=30*25

            if( (jawx1_15_3_pos < position[JAW_B2]) || (jawx2_15_3_pos < position[JAW_B1])
                || (jawy1_12_8_pos < position[JAW_A1]) || (jawy2_12_8_pos < position[JAW_A2]) )
            {
                if(g_status==RAD_ON)
                    InsertInterlockNoErr(9, SAFETY_IL, IL_READY | IL_INTERRUPT | IL_RADON, LATCHED);
                else
                    InsertInterlockNoErr(9, SAFETY_IL, IL_READY | IL_INTERRUPT | IL_RADON, NOT_LATCHED);
            }
            else
                DeleteInterlock(9);
            break;
        case RW_60:
            //field size: x*y=30*20
            if( (jawx1_15_3_pos < position[JAW_B2]) || (jawx2_15_3_pos < position[JAW_B1])
                || (jawy1_10_3_pos < position[JAW_A1]) || (jawy2_10_3_pos < position[JAW_A2]) )
            {
                if(g_status==RAD_ON)
                    InsertInterlockNoErr(9, SAFETY_IL, IL_READY | IL_INTERRUPT | IL_RADON, LATCHED);
                else
                    InsertInterlockNoErr(9, SAFETY_IL, IL_READY | IL_INTERRUPT | IL_RADON, NOT_LATCHED);
            }
            else
                DeleteInterlock(9);
            break;
        default:
            break;
        }
    }

    if(io->acc3_exist)
    {
        //qDebug()<<"acc3 ="<<io->accessory_3_type;
        switch(io->accessory_3_type)
        {
        case EA205:
            tmp_x1 = qAbs((UINT16)jawx1_6_5_pos - position[JAW_B2]);
            tmp_x2 = qAbs((UINT16)jawx2_6_5_pos - position[JAW_B1]);
            tmp_y1 = qAbs((UINT16)jawy1_6_5_pos - position[JAW_A1]);
            tmp_y2 = qAbs((UINT16)jawy2_6_5_pos - position[JAW_A2]);
            break;
        case EA210:
            tmp_x1 = qAbs((UINT16)jawx1_9_5_pos - position[JAW_B2]);
            tmp_x2 = qAbs((UINT16)jawx2_9_5_pos - position[JAW_B1]);
            tmp_y1 = qAbs((UINT16)jawy1_9_5_pos - position[JAW_A1]);
            tmp_y2 = qAbs((UINT16)jawy2_9_5_pos - position[JAW_A2]);
            break;
        case EA215:
            tmp_x1 = qAbs((UINT16)jawx1_11_5_pos - position[JAW_B2]);
            tmp_x2 = qAbs((UINT16)jawx2_11_5_pos - position[JAW_B1]);
            tmp_y1 = qAbs((UINT16)jawy1_11_5_pos - position[JAW_A1]);
            tmp_y2 = qAbs((UINT16)jawy2_11_5_pos - position[JAW_A2]);
            break;
        case EA220:
            tmp_x1 = qAbs((UINT16)jawx1_13_5_pos - position[JAW_B2]);
            tmp_x2 = qAbs((UINT16)jawx2_13_5_pos - position[JAW_B1]);
            tmp_y1 = qAbs((UINT16)jawy1_13_5_pos - position[JAW_A1]);
            tmp_y2 = qAbs((UINT16)jawy2_13_5_pos - position[JAW_A2]);
            break;
        case EA225:
            tmp_x1 = qAbs((UINT16)jawx1_16_pos - position[JAW_B2]);
            tmp_x2 = qAbs((UINT16)jawx2_16_pos - position[JAW_B1]);
            tmp_y1 = qAbs((UINT16)jawy1_16_pos - position[JAW_A1]);
            tmp_y2 = qAbs((UINT16)jawy2_16_pos - position[JAW_A2]);
            break;
        default:
            break;
        }
        //qDebug()<<tmp_x1<<tmp_x2<<tmp_y1<<tmp_y2<<tmp_colli<<tmp_gantry;
        if ((tmp_x1>jawx1_max_toler) || (tmp_x2>jawx2_max_toler)
                || (tmp_y1>jawy1_max_toler) || (tmp_y2>jawy2_max_toler))
        {
            if(g_status==RAD_ON)
                InsertInterlockNoErr(9, SAFETY_IL, IL_READY | IL_INTERRUPT | IL_RADON, LATCHED);
            else
                InsertInterlockNoErr(9, SAFETY_IL, IL_READY | IL_INTERRUPT | IL_RADON, NOT_LATCHED);
        }
        else
            DeleteInterlock(9);
    }

    tmp_x1 = qAbs( (int)(move_target_speed[JAW_B2].target_position) - (int)(position[JAW_B2]) );
    tmp_x2 = qAbs( (int)(move_target_speed[JAW_B1].target_position) - (int)(position[JAW_B1]) );
    tmp_y1 = qAbs( (int)(move_target_speed[JAW_A1].target_position) - (int)(position[JAW_A1]) );
    tmp_y2 = qAbs( (int)(move_target_speed[JAW_A2].target_position) - (int)(position[JAW_A2]) );
    tmp_colli = qAbs( (int)(move_target_speed[COLLIMATOR].target_position) - (int)(position[COLLIMATOR]) );
    tmp_gantry = qAbs( (int)(move_target_speed[GANTRY].target_position) - (int)(gantry_position_speed.gantry_position) );

    if(tmp_x1>jawx1_max_toler || tmp_x2>jawx2_max_toler
            || tmp_y1>jawy1_max_toler || tmp_y2>jawy2_max_toler)
    {
        if(g_status==RAD_ON)
            InsertInterlockNoErr(51, SAFETY_IL, IL_READY | IL_INTERRUPT | IL_RADON, LATCHED);
        else
            InsertInterlockNoErr(51, SAFETY_IL, IL_READY | IL_INTERRUPT | IL_RADON, NOT_LATCHED);
    }
    else
        DeleteInterlock(51);

    if(tmp_colli > colli_max_toler)
    {
        if(g_status==RAD_ON)
            InsertInterlockNoErr(49, SAFETY_IL, IL_READY | IL_INTERRUPT | IL_RADON, LATCHED);
        else
            InsertInterlockNoErr(49, SAFETY_IL, IL_READY | IL_INTERRUPT | IL_RADON, NOT_LATCHED);
    }
    else
        DeleteInterlock(49);

    if(tmp_gantry > gantry_max_toler)
    {
        if (!g_is_rotate_treatment)
        {
            qDebug()<<"gantry:"<<move_target_speed[GANTRY].target_position<<", "<<motor->gantry_position_speed.gantry_position<<", abs ="<<tmp_gantry;
            if(g_status==RAD_ON)
                InsertInterlockNoErr(10, SAFETY_IL, IL_READY | IL_INTERRUPT | IL_RADON, LATCHED);
            else
                InsertInterlockNoErr(10, SAFETY_IL, IL_READY | IL_INTERRUPT | IL_RADON, NOT_LATCHED);
        }
    }
    else
        DeleteInterlock(10);

//    qDebug()<<"x1:"<<move_target_speed[JAW_B2].target_position<<"  "<<position[JAW_B2]<<"  abs ="<<tmp_x1;
//    qDebug()<<"x2:"<<move_target_speed[JAW_B1].target_position<<"  "<<position[JAW_B1]<<"  abs ="<<tmp_x2;
//    qDebug()<<"y1:"<<move_target_speed[JAW_A1].target_position<<"  "<<position[JAW_A1]<<"  abs ="<<tmp_y1;
//    qDebug()<<"y2:"<<move_target_speed[JAW_A2].target_position<<"  "<<position[JAW_A2]<<"  abs ="<<tmp_y2;
//    qDebug()<<"colli:"<<move_target_speed[COLLIMATOR].target_position<<", "<<position[COLLIMATOR]<<", abs="<<tmp_colli;
//    qDebug()<<"gantry:"<<move_target_speed[GANTRY].target_position<<", "<<motor->gantry_position_speed.gantry_position<<", abs ="<<tmp_gantry;

}

bool Motor::AnalysRevFram(TransRecvT& revdata)
{

    if (revdata.data[1]=='C')
    {
        rev_frame_num = 0;
    }

    if (!ProcessCHKIL(revdata))
        return false;

    switch(revdata.data[1])
    {
    case '1':
        return RevJawColliPosition(revdata);
    case '2':
        return RevGantryPosnSpeed(revdata);
    case '3':
        return RevSpeedTest(revdata);
    case 'C':
        return RevResetEcho(revdata);
    case 'E':
        return RevErrorReport(revdata);
    case 'K':
        return RevGantryCompletionInRadOn(revdata);
    case 'Q':
        return RevVirtualWedgeSpeedTestComplete(revdata);
    case 'S':
        return RevSpeedTestComplete(revdata);
    case 'W':
        return RevCheckReport(revdata);
    case 'Y':
        return RevEmergencyLineStatus(revdata);
    default:   //Invalid command type received from controller 0
        InsertInterlock(79, 950, ERROR_IL);
    }

    return false;
}

bool Motor::RevJawColliPosition(TransRecvT &revdata)
{
    if (revdata.len != 14)
    {
        InsertInterlock(79, 960, ERROR_IL);   //Invalid length for command type from controller 0 .
        return false;
    }

    position[JAW_A1] = (revdata.data[2] & 0xFF) | ((revdata.data[3] << 8) & 0xFF00);
    position[JAW_A2] = (revdata.data[4] & 0xFF) | ((revdata.data[5] << 8) & 0xFF00);
    position[JAW_B1] = (revdata.data[6] & 0xFF) | ((revdata.data[7] << 8) & 0xFF00);
    position[JAW_B2] = (revdata.data[8] & 0xFF) | ((revdata.data[9] << 8) & 0xFF00);
    position[COLLIMATOR] = (revdata.data[10] & 0xFF) | ((revdata.data[11] << 8) & 0xFF00);

    emit base->ShowActJawColliGanSig(0);
    hand->UpdatePositionToHand();

    //LJY-12-13
    if(!init_jawcolli_target_pos)
    {
        move_target_speed[JAW_A1].target_position = position[JAW_A1];
        move_target_speed[JAW_A2].target_position = position[JAW_A2];
        move_target_speed[JAW_B1].target_position = position[JAW_B1];
        move_target_speed[JAW_B2].target_position = position[JAW_B2];
        move_target_speed[COLLIMATOR].target_position = position[COLLIMATOR];
        init_jawcolli_target_pos = true;
    }

    return true;
}

bool Motor::RevGantryPosnSpeed(TransRecvT &revdata)
{
    if (revdata.len != 8)
    {
        InsertInterlock(79, 960, ERROR_IL);   //Invalid length for command type from controller 0 .
        return false;
    }

    gantry_position_speed.gantry_position =
            (revdata.data[2] & 0xFF) | ((revdata.data[3] << 8) & 0xFF00);
    gantry_position_speed.sampled_speed = revdata.data[4];
    gantry_position_speed.expected_speed = revdata.data[5];

    emit base->ShowActJawColliGanSig(1);

    if(!init_gantry_target_pos)
    {
        move_target_speed[GANTRY].target_position = gantry_position_speed.gantry_position;
        init_gantry_target_pos = true;
    }

    ProcessGantryPosIL();
    return true;
}

bool Motor::RevSpeedTest(TransRecvT &revdata)
{
    UINT8 part;
    if (revdata.len != 9)
    {
        InsertInterlock(79, 960, ERROR_IL);   //Invalid length for command type from controller 0 .
        return false;
    }

    part = revdata.data[2];
    if ( part > GANTRY)
        return false;

    speed_test[part].position = (revdata.data[3] & 0xFF) | ((revdata.data[4] << 8) & 0xFF00);
    speed_test[part].timestamp = (revdata.data[5] & 0xFF) | ((revdata.data[6] << 8) & 0xFF00);

    return true;
}

bool Motor::RevResetEcho(TransRecvT& revdata)
{

    if (revdata.len != 9)
    {
        InsertInterlock(79, 960, ERROR_IL);   //Invalid length for command type from controller 0 .
        return false;
    }

    if (revdata.data[2] != 0)
        return false;
    memcpy(&prom_version, &(revdata.data[3]), sizeof (prom_version));

    g_c0_echo_bits_lock.lockForWrite();
    g_c0_echo_bits.setBit(0, true);
    g_c0_echo_bits_lock.unlock();
    return true;
}

bool Motor::RevErrorReport(TransRecvT &revdata)
{
    if (revdata.len == 5)
        error_now.xx = revdata.data[2];

    else if (revdata.len == 7)
    {
        error_now.xx = revdata.data[2];
        error_now.yy = revdata.data[3];
        error_now.zz = revdata.data[4];
    }
    else
    {
        InsertInterlock(79, 960, ERROR_IL);   //Invalid length for command type from controller 0 .
        return false;
    }

    ProcessErrorReportIL();
    return true;
}

bool Motor::RevGantryCompletionInRadOn(TransRecvT& revdata)
{
    if (revdata.len != 8)
    {
        InsertInterlock(79, 960, ERROR_IL);   //Invalid length for command type from controller 0 .
        return false;
    }

    gantry_completion.gantry_position =
            (revdata.data[2] & 0xFF) | ((revdata.data[3] << 8) & 0xFF00);
    gantry_completion.sampled_speed = revdata.data[4];
    gantry_completion.expected_speed =  revdata.data[5];

    emit base->ShowActJawColliGanSig(1);

    g_gantry_timer->stop();

    if (g_status == RAD_ON)
        g_is_complete = true;

    return true;
}

bool Motor::RevSpeedTestComplete(TransRecvT &revdata)
{
    UINT8 part;
    if (revdata.len != 9)
    {
        InsertInterlock(79, 960, ERROR_IL);   //Invalid length for command type from controller 0 .
        return false;
    }

    part = revdata.data[2];
    if ( part > GANTRY)
        return false;

    speed_complete[part].requested_position =
            (revdata.data[3] & 0xFF) | ((revdata.data[4] << 8) & 0xFF00);
    speed_complete[part].actual_position =
            (revdata.data[5] & 0xFF) | ((revdata.data[6] << 8) & 0xFF00);

    return true;
}

bool Motor::RevVirtualWedgeSpeedTestComplete(TransRecvT& revdata)
{
    UINT8 part;
    if (revdata.len != 9)
    {
        InsertInterlock(79, 960, ERROR_IL);   //Invalid length for command type from controller 0 .
        return false;
    }

    part = revdata.data[2];
    if (part > JAW_B2)
        return false;

    speed_virtual_wedge_complete[part].requested_position =
            (revdata.data[3] & 0xFF) | ((revdata.data[4] << 8) & 0xFF00);
    speed_virtual_wedge_complete[part].actual_position =
            (revdata.data[5] & 0xFF) | ((revdata.data[6] << 8) & 0xFF00);

    return true;
}

bool Motor::RevCheckReport(TransRecvT& revdata)
{
    CHECKREQ_TYPE_T check_status;
    if (revdata.len != 5)
    {
        InsertInterlock(79, 960, ERROR_IL);   //Invalid length for command type from controller 0 .
        return false;
    }

    check_report = (CHECKREPORT_MOTOR_T)revdata.data[2];

    g_check_index_mutex.lock();
    check_status = g_check_index;
    g_check_index_mutex.unlock();

    if(check_status==CHECK2)
    {
//        accui->SetActChkDispReport0(check_report);
        emit base->ShowActSvcValueSig(CHK_0);
    }

    if (check_report == CHECKPASSED)
    {
        g_check_bits_mutex.lock();
        g_check_bits.setBit(0, true);
        g_check_bits_mutex.unlock();
    }
    else if (check_report == RAMFAILED)
    {
        //interlock
        ;
    }
    else if (check_report == ROMFAILED)
    {
        //interlock
        ;
    }
    else if (check_report == WATCHDOGFAILED)
    {
        //interlock
        ;
    }

    return true;
}

bool Motor::RevEmergencyLineStatus(TransRecvT& revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 960, ERROR_IL);   //Invalid length for command type from controller 0 .
        return false;
    }

    line_status = (LINE_STATUS_T)revdata.data[2];
//    accui->SetActT1DispReport0(line_status);
    emit base->ShowActSvcValueSig(T1_0);

    return true;
}

void Motor::ProcessGantryPosIL()
{
    //顺时针方向16进制值减小

//    qDebug()<<"target_position=="<<move_target_speed[GANTRY].target_position
//           <<"  gantry_position"<<gantry_position_speed.gantry_position;
//    if (  gantry_position_speed.gantry_position <
//          (move_target_speed[GANTRY].target_position - 388))
//        InsertInterlock(10, SAFETY_IL, IL_READY|IL_INTERRUPT|IL_RADON,
//                        RADON_LATCHED);
//    else
//    {
//        if (g_status != RAD_ON)
//            DeleteInterlock(10);
//    }
    if ( g_is_rotate_treatment)
    {
      //  g_gantry_timer->start(2400);
        emit base->g_gantry_start_signal();
        qDebug()<<"rotate timer";
    }
//    else
//    {
//        gantry_timer->stop();
//    }
}

void Motor::ProcessErrorReportIL()
{
    switch (error_now.xx)
    {
    case 0x34:
        InsertInterlock(33, 52, SAFETY_IL, IL_RADON, LATCHED);   //安全联锁
        break;
    case 0x00:      //Jaw Y1 Y2 read/write test fail 非
        InsertInterlock(63, 0, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x01:       //JawY1 F-to-D conversation test fail   非
        InsertInterlock(63, 1, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x02:       // Possible jaw Y1 collision or protentiometer failure .
        InsertInterlock(63, 2, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x03:       //JawY1 DAC-Drive-ADC test fail 非
        InsertInterlock(63, 3, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x07:       //Jaw Y1 speed control error during speed test .
        InsertInterlock(63, 7, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x0B:      //JawY2 F-to-D conversation test fail 非
        InsertInterlock(63, 11, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x0C:      //Possible jaw Y2 collision or protentiometer failure .
        InsertInterlock(63, 12, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x0D:      //JawY2 DAC-Drive-ADC test fail 非
        InsertInterlock(63, 13, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x11:      //Jaw Y2 speed control error during speed test .
        InsertInterlock(63, 17, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x14:      //Jaw X1 X2 read/write test fail 非
        InsertInterlock(63, 20, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x15:      //Jaw X2 F-to-D conversation test fail 非
        InsertInterlock(63, 21, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x16:      //Possible jaw X2 collision or protentiometer failure .
        InsertInterlock(63, 22, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x17:      //JawX2 DAC-Drive-ADC test fail 非
        InsertInterlock(63, 23, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x1B:      //Jaw X2 speed control error during speed test .
        InsertInterlock(63, 27, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x1F:      //JawX1 F-to-D conversation test fail   非
        InsertInterlock(63, 31, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x20:      //Possible jaw X1 collision or protentiometer failure .
        InsertInterlock(63, 32, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x21:      //JawX1 DAC-Drive-ADC test fail 非
        InsertInterlock(63, 33, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x25:      //Jaw X1 speed control error during speed test .
        InsertInterlock(63, 37, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x28:      //collimator/gantry read/write test fail 非
        InsertInterlock(63, 40, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x29:      //collimator F-to-D conversation test fail 非
        InsertInterlock(63, 41, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x2B:      //collimator DAC-Drive-ADC test fail 非
        InsertInterlock(63, 43, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x2F:      //Collimator speed control error during speed test .
        InsertInterlock(63, 47, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x33:      // gantry F-to-D conversation test fail 非
        InsertInterlock(63, 51, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x35:      // gantry AC-Drive-ADC test fail 非
        InsertInterlock(63, 53, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x36:      //Arc hardware backup monitor stopped rotational treatment 安全联锁
        InsertInterlock(63, 54, ERROR_IL, IL_RADON, LATCHED);
        break;
    case 0x38:      //Gantry speed check interlock during rotational treatment 安全联锁
        InsertInterlock(63, 56, ERROR_IL, IL_RADON, LATCHED);
        break;
    case 0x39:      //Gantry speed control error during speed rest .
        InsertInterlock(63, 57, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x5f:      //Illegal data received by controller 0 安全联锁        //XXX
        InsertInterlock(79, 96, ERROR_IL);
        break;
    case 0x60:      //INvalid data received by controller 0 安全联锁
        InsertInterlock(79, 96, ERROR_IL);
        break;
    case 0x61:      //Frame-# and checksum received by controller 0 are wrong 安全联锁
        InsertInterlock(79, 97, ERROR_IL);
        break;
    case 0x62:      //Checksum received by controller 0 is wrong 安全联锁
        InsertInterlock(79, 98, ERROR_IL);
        break;
    case 0x63:      //Frame-# received by controller 0 is wrong. 安全联锁
        InsertInterlock(79, 99, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x05:      //A change in jaw Y1 position (at least ← 0.2 cm) without command .安全
        InsertInterlock(87, 5, SAFETY_IL, IL_RADON|IL_PROGRAM, LATCHED);
        break;
    case 0x06:      //Jaw Y1 position wrong during motion .安全
        InsertInterlock(87, 6, SAFETY_IL, IL_RADON|IL_PROGRAM, LATCHED);
        break;
    case 0x0F:      //A change in jaw Y2 position (at least ← 0.2 cm) without command .安全
        InsertInterlock(88, 15);
        break;
    case 0x10:      //Jaw Y2 position wrong during motion . 安全
        InsertInterlock(88, 16, SAFETY_IL, IL_PROGRAM | IL_RADON, LATCHED);
        break;
    case 0x19:      //A change in jaw X2 position (at least ← 0.2 cm) without command .安全
        InsertInterlock(89, 25);
        break;
    case 0x1A:      // Jaw X2 position wrong during motion .安全
        InsertInterlock(89, 26, SAFETY_IL, IL_RADON|IL_PROGRAM, LATCHED);
        break;
    case 0x23:      //A change in jaw X1 position (at least ← 0.2 cm) without command .安全
        InsertInterlock(90, 35);
        break;
    case 0x24:      //Jaw X1 position wrong during motion .安全
        InsertInterlock(90, 36, SAFETY_IL, IL_RADON|IL_PROGRAM, LATCHED);
        break;
    case 0x2D:      //Change in collimator angle (at least 21 counts) without command .安全
        InsertInterlock(91, 45);
        break;
    case 0x37:      //Change in gantry angle (at least 21 counts) without command.安全
        InsertInterlock(92, 55);
        break;

    //LJY-12-23
    case 0x3d:      //RAM test failed.
        InsertInterlock(63, 61, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x3e:      //ROM test failed.
        InsertInterlock(63, 62, ERROR_IL, IL_ALL, LATCHED);
        break;

    //LJY-12-24
    case 0x04:
    case 0x08:
    case 0x09:
    case 0x0E:
    case 0x12:
    case 0x13:
    case 0x18:
    case 0x1C:
    case 0x1D:
    case 0x22:
    case 0x26:
    case 0x27:
    case 0x2A:
    case 0x2C:
    case 0x30:
    case 0x31:
    case 0x3A:
    case 0x3B:
    case 0x3C:
    case 0x3F:
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x45:
        InsertInterlock(63, error_now.xx, ERROR_IL, IL_ALL, LATCHED);
        break;
    default:        //Invalid data received from controller 0 安全联锁
        InsertInterlock(79, 970, ERROR_IL);
        break;
    }
}

void Motor::ProcessJawnColliPositionIL()
{
    if (position[JAW_A1] > move_target_speed[JAW_A1].target_position + (0.3  / jaw_y1_gain) ||
            position[JAW_A2] > move_target_speed[JAW_A2].target_position + (0.3  / jaw_y2_gain))
        InsertInterlockNoErr(9, SAFETY_IL, IL_RADON|IL_READY|IL_INTERRUPT,
                        RADON_LATCHED);       //???
    else
    {
        if (g_status != RAD_ON)
            DeleteInterlock(9);
    }
    //#49:cllimator position more than 1 degree
//    if (position[COLLIMATOR] > move_target_speed[COLLIMATOR].target_position)
//        InsertInterlock(49, SAFETY_IL, IL_READY|IL_INTERRUPT|IL_RADON,
//                        RADON_LATCHED);    //安全联锁
    //#51:actual position more than 3 mm
//    if (position[JAW_A1] > move_target_speed[JAW_A1].target_position + (0.3  / jaw_y1_gain)
//            || position[JAW_A2] > move_target_speed[JAW_A2].target_position + (0.3  / jaw_y2_gain)
//            || position[JAW_B1] > move_target_speed[JAW_B1].target_position + (0.3  / jaw_x2_gain)
//            || position[JAW_B2] > move_target_speed[JAW_B2].target_position + (0.3  / jaw_x1_gain))
//        InsertInterlock(51, SAFETY_IL, IL_READY|IL_INTERRUPT|IL_RADON, RADON_LATCHED);    //安全联锁
}

bool Motor::ProcessCHKIL(TransRecvT& revdata)
{
    UINT8 fc;
    if ((ComputeLstCHK(revdata.data, revdata.len - 1) & 0x7F) !=
            (revdata.data[revdata.len - 1] & 0x7F))
        return false;

    fc = ComputeRevCHK(revdata.data, revdata.len - 2);
    if (fc != revdata.data[revdata.len - 2])
    {
        if ((fc & 0xF0) != (revdata.data[revdata.len - 2] & 0xF0))
            InsertInterlock(79, 930, ERROR_IL);       // Invalid frame number detected from controller 0 安全联锁
        if ((fc & 0x0F) !=  (revdata.data[revdata.len - 2] & 0x0F))
            InsertInterlock(79, 940, ERROR_IL);       //Invalid checksum detected from controller 0 安全联锁

        return false;
    }
    return true;
}

bool Motor::CheckConvertGantry(float preset_gpos, UINT32& gantry_pst)
{
    bool suc;
    float gpos,gantry;
    gpos = base->ConvertMemberToFloat(motor->gantry_position_speed.gantry_position, GANTRY_CK);
    gantry = preset_gpos;
    if(gpos>=0)
    {
        if (gantry > (180 + GANTRY_LIMIT))
        {
            gantry  = gantry - 360;
        }
    }
    else
    {
        if (gantry >= (180 - GANTRY_LIMIT))     //12-01 if (gantry > (180 - GANTRY_LIMIT))
        {
            gantry = gantry - 360;
        }
    }

    if (gantry > gpos)
        emit base->GantryDirecSignal(true);
    else
        emit base->GantryDirecSignal(false);

    suc = SlaveBase::CheckValue(gantry, GANTRY_CK, gantry_pst);
    if(!suc)
    {
        //erro
        return false;
    }
    else
        return true;
}

//void Motor::InsertGantryTimeoutIL()
//{
//    qDebug()<<"hello";
//}
