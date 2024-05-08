
#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"
using namespace std;

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();
    void setRect(const int& x, const int& y) //set kich thuoc cho rect
    {
        rect_.x = x;
        rect_.y = y;
    }
    SDL_Rect GetRect() const {return rect_;} //lay ra rect
    SDL_Texture* GetObject() const {return p_object_;} // lay ra boject

    virtual bool LoadImg(string path, SDL_Renderer* screen); //load anh <dua ve ham ao>
    void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
    void Free();//giai phong doi tuong

protected:
    SDL_Texture* p_object_;  //luu tru anh
    SDL_Rect rect_; // luu kich thuoc anh

};

#endif // BASE_OBJECT_H_
