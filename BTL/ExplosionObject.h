#ifndef EXP_H_
#define EXP_H_

#include "CommonFunc.h"
#include "BaseObject.h"

#define NUM_FRAME_EXP 8

using namespace std;

class ExplosionObject : public BaseObject
{

public:
    ExplosionObject();
    ~ExplosionObject();

    void set_clips();
    void set_frame(const int& fr) {frame_ = fr;}
    virtual bool LoadImg (string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* screen);
    int get_frame_width() const {return frame_width_;}
    int get_frame_height() const {return frame_height_;}

    void SetRect(const int& x, const int& y) //set kich thuoc cho rect
    {
        rect_.x = x;
        rect_.y = y;
    }
private:
    int frame_width_;
    int frame_height_;

    unsigned int frame_;
    SDL_Rect frame_clip_[8];

};
#endif // EXP_H_
