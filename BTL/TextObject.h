
#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include "CommonFunc.h"

using namespace std;

class TextObject
{
public:
    TextObject();
    ~TextObject();

    enum TextColor
    {
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,
    };
    bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);
    void Free();

    void SetColor(Uint8 red, Uint8 green, Uint8 blue);
    void SetColor(int type);

    void RenderText(SDL_Renderer* screen,
                    int xp, int yp,
                    SDL_Rect* clip = NULL,
                    double angle=0.0,
                    SDL_Point* center = NULL,
                    SDL_RendererFlip flip = SDL_FLIP_NONE); // in text ra man hinh

    int GetWidth() const {return width_;}
    int GetHeight() const {return height_;}

    void SetText(const string & text) {str_val_ = text;}
    string Gettext() const {return str_val_;}


private:
    string str_val_;  // save content
    SDL_Color text_color_; // save color
    SDL_Texture* texture_; // manage text
    int width_;
    int height_;


};

#endif // TEXT_OBJECT_H
