
#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"
#include <vector>

using namespace std;

#define GRAVITY_SPEED 0.8; //toc do roi
const int MAX_FALL_SPEED = 10;
#define PLAYER_SPEED 8
#define PLAYER_JUMP_VAL 20

using namespace std;

class MainObject : public BaseObject //ke thua
{
public:
    MainObject();
    ~MainObject();

    enum WalkType
    {
        WALK_NONE = 0,
        WALK_RIGHT = 1,
        WALK_LEFT = 2,
    };

    bool LoadImg(string path, SDL_Renderer* screen); //thong so kich thuoc anh
    void Show(SDL_Renderer* des); //hien thi tung flame
    void HandelInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* bullet_sound[2]); // xu li su kien
    void set_clips(); // hieu ung animation

    void DoPlayer(Map& map_data); // cong tru 1 luong cho nv di chuyen
    void CheckToMap(Map& map_data, Mix_Chunk* deep_sound[2]); // xu li va cham
    void SetMapXY(const int map_x, const int map_y) {map_x_ = map_x; map_y_ = map_y;}
    void CenterEntityOnMap(Map& map_data); // tinh toan thong so cua map khi di chuyen
    void UpdateImagePlayer(SDL_Renderer* des);
    SDL_Rect GetRectFrame();

    int get_frame_width() const {return width_frame_;}
    int get_frame_height() const {return height_frame_;}

    void set_bullet_list(vector<BulletObject*> bullet_list)
    {
        p_bullet_list_ = bullet_list;
    }

    vector<BulletObject*> get_bullet_list() const {return p_bullet_list_;}
    void HandleBullet(SDL_Renderer* des);
    void RemoveBullet(const int& idx);
    void IncreaseMoney(); // xu li tien
    void set_comeback_time(const int& cb_time) {come_back_time_ = cb_time;}

    int GetMoneyCount() const {return money_count;}

private:
    int money_count; // so luong tien an duoc

    vector<BulletObject*> p_bullet_list_;

    float x_val_;      //vi tri tien lui bao nhieu
    float y_val_;

    float x_pos_;      //vi tri hien tai cua nhan vat
    float y_pos_;

    int width_frame_;   //kich thuoc cua mot frame
    int height_frame_;

    SDL_Rect frame_clip_[8]; // luu cac frame
    Input input_type_;  // luu cac trang thai
    int frame_; // luu chi so frame
    int status_; // luu kieu di chuyen <trai -phai>
    bool on_ground_;

    int map_x_; //luu vi tri ban do
    int map_y_;

    int come_back_time_;
};
#endif // MAIN_OBJECT_H_
