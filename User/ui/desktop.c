#include "desktop.h"
#include "lvgl.h"

LV_IMG_DECLARE(music);
LV_IMG_DECLARE(pic);
LV_IMG_DECLARE(file);
LV_IMG_DECLARE(setting);
LV_IMG_DECLARE(video);
LV_IMG_DECLARE(network);
LV_IMG_DECLARE(qq);
LV_IMG_DECLARE(Wechat);
LV_IMG_DECLARE(calendar);
void desktop_ui_create(void *parent)
{

    static lv_style_t style;
    static lv_obj_t * rect;
    static lv_obj_t * btn1;
    static lv_obj_t * imgbtn[3][6];
    uint32_t i,j;

    //窗口
    rect= lv_obj_create(parent,NULL);
    lv_obj_set_size(rect,lv_obj_get_width(parent),lv_obj_get_height(parent));
    lv_style_copy(&style, &lv_style_scr);
    style.body.main_color = LV_COLOR_BLACK;
    style.body.grad_color=LV_COLOR_BLACK;
    lv_obj_set_style(rect,&style);

    //顶部状态栏
    lv_obj_t * top_rect=lv_obj_create(rect,NULL);
    lv_obj_set_size(top_rect,lv_obj_get_width(rect),40);
    lv_obj_set_pos(top_rect,0,0);

    //宫格图标
    /*Create style to make the button darker when pressed*/
    lv_style_t style_pr;
    lv_style_copy(&style_pr, &lv_style_plain);
    style_pr.image.color = LV_COLOR_BLACK;
    style_pr.image.intense = LV_OPA_50;
    style_pr.text.color = LV_COLOR_HEX3(0xaaa);

    /*Create an Image button*/
  /*  lv_obj_t * imgbtn1 = lv_imgbtn_create(rect, NULL);
    lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_REL, &music);
    lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PR, &music);
    lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_REL, &music);
    lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_PR, &music);
    lv_imgbtn_set_style(imgbtn1, LV_BTN_STATE_PR, &style_pr);
    lv_imgbtn_set_style(imgbtn1, LV_BTN_STATE_TGL_PR, &style_pr);
    lv_imgbtn_set_toggle(imgbtn1, true);
    lv_obj_align(imgbtn1,top_rect,LV_ALIGN_OUT_BOTTOM_LEFT, 0,0);
*/
    for(j=0;j<3;j++)
    {
        for(i=0;i<6;i++)
        {
            imgbtn[j][i] = lv_imgbtn_create(rect, NULL);
            lv_imgbtn_set_src(imgbtn[j][i], LV_BTN_STATE_REL, &music);
            lv_imgbtn_set_src(imgbtn[j][i], LV_BTN_STATE_PR, &music);
            lv_imgbtn_set_src(imgbtn[j][i], LV_BTN_STATE_TGL_REL, &music);
            lv_imgbtn_set_src(imgbtn[j][i], LV_BTN_STATE_TGL_PR, &music);
            lv_imgbtn_set_style(imgbtn[j][i], LV_BTN_STATE_PR, &style_pr);        /*Use the darker style in the pressed state*/
            lv_imgbtn_set_style(imgbtn[j][i], LV_BTN_STATE_TGL_PR, &style_pr);
            lv_imgbtn_set_toggle(imgbtn[j][i], true);
        }
    }
    lv_imgbtn_set_src(imgbtn[0][0], LV_BTN_STATE_REL, &qq);
    lv_imgbtn_set_src(imgbtn[0][0], LV_BTN_STATE_PR, &qq);
    lv_imgbtn_set_src(imgbtn[0][0], LV_BTN_STATE_TGL_REL, &qq);
    lv_imgbtn_set_src(imgbtn[0][0], LV_BTN_STATE_TGL_PR, &qq);
    lv_imgbtn_set_style(imgbtn[0][0], LV_BTN_STATE_PR, &style_pr);
    lv_imgbtn_set_style(imgbtn[0][0], LV_BTN_STATE_TGL_PR, &style_pr);

    lv_imgbtn_set_src(imgbtn[0][1], LV_BTN_STATE_REL, &pic);
    lv_imgbtn_set_src(imgbtn[0][1], LV_BTN_STATE_PR, &pic);
    lv_imgbtn_set_src(imgbtn[0][1], LV_BTN_STATE_TGL_REL, &pic);
    lv_imgbtn_set_src(imgbtn[0][1], LV_BTN_STATE_TGL_PR, &pic);
    lv_imgbtn_set_style(imgbtn[0][1], LV_BTN_STATE_PR, &style_pr);
    lv_imgbtn_set_style(imgbtn[0][1], LV_BTN_STATE_TGL_PR, &style_pr);

    lv_imgbtn_set_src(imgbtn[0][2], LV_BTN_STATE_REL, &file);
    lv_imgbtn_set_src(imgbtn[0][2], LV_BTN_STATE_PR, &file);
    lv_imgbtn_set_src(imgbtn[0][2], LV_BTN_STATE_TGL_REL, &file);
    lv_imgbtn_set_src(imgbtn[0][2], LV_BTN_STATE_TGL_PR, &file);
    lv_imgbtn_set_style(imgbtn[0][2], LV_BTN_STATE_PR, &style_pr);
    lv_imgbtn_set_style(imgbtn[0][2], LV_BTN_STATE_TGL_PR, &style_pr);

    lv_imgbtn_set_src(imgbtn[0][3], LV_BTN_STATE_REL, &setting);
    lv_imgbtn_set_src(imgbtn[0][3], LV_BTN_STATE_PR, &setting);
    lv_imgbtn_set_src(imgbtn[0][3], LV_BTN_STATE_TGL_REL, &setting);
    lv_imgbtn_set_src(imgbtn[0][3], LV_BTN_STATE_TGL_PR, &setting);
    lv_imgbtn_set_style(imgbtn[0][3], LV_BTN_STATE_PR, &style_pr);
    lv_imgbtn_set_style(imgbtn[0][3], LV_BTN_STATE_TGL_PR, &style_pr);

    lv_imgbtn_set_src(imgbtn[0][4], LV_BTN_STATE_REL, &video);
    lv_imgbtn_set_src(imgbtn[0][4], LV_BTN_STATE_PR, &video);
    lv_imgbtn_set_src(imgbtn[0][4], LV_BTN_STATE_TGL_REL, &video);
    lv_imgbtn_set_src(imgbtn[0][4], LV_BTN_STATE_TGL_PR, &video);
    lv_imgbtn_set_style(imgbtn[0][4], LV_BTN_STATE_PR, &style_pr);
    lv_imgbtn_set_style(imgbtn[0][4], LV_BTN_STATE_TGL_PR, &style_pr);

    lv_imgbtn_set_src(imgbtn[0][5], LV_BTN_STATE_REL, &network);
    lv_imgbtn_set_src(imgbtn[0][5], LV_BTN_STATE_PR, &network);
    lv_imgbtn_set_src(imgbtn[0][5], LV_BTN_STATE_TGL_REL, &network);
    lv_imgbtn_set_src(imgbtn[0][5], LV_BTN_STATE_TGL_PR, &network);
    lv_imgbtn_set_style(imgbtn[0][5], LV_BTN_STATE_PR, &style_pr);
    lv_imgbtn_set_style(imgbtn[0][5], LV_BTN_STATE_TGL_PR, &style_pr);

    lv_imgbtn_set_src(imgbtn[1][0], LV_BTN_STATE_REL, &Wechat);
    lv_imgbtn_set_src(imgbtn[1][0], LV_BTN_STATE_PR, &Wechat);
    lv_imgbtn_set_src(imgbtn[1][0], LV_BTN_STATE_TGL_REL, &Wechat);
    lv_imgbtn_set_src(imgbtn[1][0], LV_BTN_STATE_TGL_PR, &Wechat);
    lv_imgbtn_set_style(imgbtn[1][0], LV_BTN_STATE_PR, &style_pr);
    lv_imgbtn_set_style(imgbtn[1][0], LV_BTN_STATE_TGL_PR, &style_pr);

    lv_imgbtn_set_src(imgbtn[1][1], LV_BTN_STATE_REL, &calendar);
    lv_imgbtn_set_src(imgbtn[1][1], LV_BTN_STATE_PR, &calendar);
    lv_imgbtn_set_src(imgbtn[1][1], LV_BTN_STATE_TGL_REL, &calendar);
    lv_imgbtn_set_src(imgbtn[1][1], LV_BTN_STATE_TGL_PR, &calendar);
    lv_imgbtn_set_style(imgbtn[1][1], LV_BTN_STATE_PR, &style_pr);
    lv_imgbtn_set_style(imgbtn[1][1], LV_BTN_STATE_TGL_PR, &style_pr);

    lv_obj_align(imgbtn[0][0],rect,LV_ALIGN_IN_TOP_LEFT, 40,60);
    lv_obj_align(imgbtn[1][0],rect,LV_ALIGN_IN_LEFT_MID, 40,0);
    lv_obj_align(imgbtn[2][0],rect,LV_ALIGN_IN_BOTTOM_LEFT, 40,-40);

    for(j=0;j<3;j++)
    {
        for(i=1;i<6;i++)
        {
            lv_obj_align(imgbtn[j][i],imgbtn[j][i-1],LV_ALIGN_OUT_RIGHT_MID, 40,0);
        }
    }


}
