
#include <iostream>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "PlayerPower.h"
#include "Geometric.h"
#include "BossObject.h"

BaseObject g_background;
TTF_Font* font_time = NULL;

using namespace std;

bool InitData()  //khoi dong thong so cho moi truong SDL
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO); //thiet lap mtrg ban dau cho SDL
    if (ret < 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");//che do chat luong ti le

    g_window = SDL_CreateWindow("Game Cpp SDL",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_CENTERED,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);
    if (g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }
        if (TTF_Init() == -1)
        {
            success = false;
        }

        font_time = TTF_OpenFont("font//dlxfont.ttf", 15);
        if (font_time == NULL)
        {
            success = false;
        }

        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
            return false;
        //Read file audio
        g_sound_bullet[0] = Mix_LoadWAV("Fire.wav");
        g_sound_bullet[1] = Mix_LoadWAV("Laser.wav");
        g_sound_exp[0] = Mix_LoadWAV("explosion.wav");
        g_sound_exp[1] = Mix_LoadWAV("beep.wav");
        if (g_sound_bullet[0] == NULL || g_sound_bullet[1] == NULL || g_sound_exp[0] == NULL || g_sound_exp[1] == NULL)
            return false;

    }
    return success;
}

void close()
{
    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}
bool LoadBackground()
{
    bool ret = g_background.LoadImg("img/background1.png", g_screen);
    if (ret == false)
        return false;
    return true;
}
vector<ThreatsObject*> MakeThreadList()
{
    vector <ThreatsObject*> list_threats;

    ThreatsObject* dynamic_threats = new ThreatsObject[20];

    for (int i = 0; i < 20; i++)
    {
        ThreatsObject* p_threat = (dynamic_threats + i);
        if (p_threat != NULL)
        {
            p_threat -> LoadImg("img//threat_left.png", g_screen);
            p_threat -> set_clips();
            p_threat -> set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT); // kieu threat
            p_threat -> set_x_pos(500 + i * 500);   //vi tri roi threat
            p_threat -> set_y_pos(200);

            int pos1 = p_threat -> get_x_pos() - 60;
            int pos2 = p_threat -> get_x_pos() + 60;
            p_threat -> SetAnimationPos(pos1, pos2);
            p_threat -> set_input_left(1);

            list_threats.push_back(p_threat);
        }
    }

    ThreatsObject* threats_objs = new ThreatsObject[20];

    for (int i = 0; i < 20; i++)
    {
        ThreatsObject* p_threat = (threats_objs + i);
        if (p_threat != NULL)
        {
            p_threat -> LoadImg("img//threat_level.png", g_screen);
            p_threat -> set_clips();
            p_threat -> set_type_move(ThreatsObject::STATIC_THREAT);  //kieu threat
            p_threat -> set_x_pos(700 + i * 1200); // vi tri roi cua thread
            p_threat -> set_y_pos(250);
            p_threat -> set_input_left(0);

            BulletObject* p_bullet = new BulletObject();
            p_threat -> InitBullet(p_bullet, g_screen);

            list_threats.push_back(p_threat);
        }
    }
    return list_threats;
}
int main(int argc, char * argv[])
{
    ImpTimer fps_timer;

    if (InitData() == false)
        return -1;
    if (LoadBackground() == false)
        return -1;

    GameMap game_map;
    game_map.LoadMap("map/map01.dat");
    game_map.LoadTiles(g_screen);

    MainObject p_player;
    p_player.LoadImg("img//player_right.png", g_screen);
    p_player.set_clips();

    PlayerPower player_power;
    player_power.Init(g_screen);

    PlayerMoney player_money;
    player_money.Init(g_screen);
    player_money.setPos(SCREEN_WIDTH * 0.5 - 300, 8);

    vector<ThreatsObject*> threats_list = MakeThreadList();

    //Boss Threat
    BossObject bossObject;
    bool ret = bossObject.LoadImg("img//boss_object.png", g_screen);
    bossObject.set_clips();
    int xPosBoss = MAX_MAP_X * TILE_SIZE - SCREEN_WIDTH * 0.6;
    bossObject.set_x_pos(xPosBoss);
    bossObject.set_y_pos(10);


    ExplosionObject exp_threat;
    bool tRet = exp_threat.LoadImg("img//exp3.png", g_screen);
    if (!tRet) return -1;
    exp_threat.set_clips();

    ExplosionObject exp_main;
    bool bRet = exp_main.LoadImg("img//exp3.png", g_screen);
    if (!bRet) return -1;
    exp_main.set_clips();

    int num_die = 0;

    //Time text
    TextObject time_game; // hien thi thoi gian choi game
    time_game.SetColor(TextObject::WHITE_TEXT);

    TextObject mark_game; // hien thi so diem
    mark_game.SetColor(TextObject::WHITE_TEXT);
    UINT mark_value = 0;

    TextObject money_game;
    money_game.SetColor(TextObject::WHITE_TEXT);


    bool is_quit = false;
    while(!is_quit)
    {
        fps_timer.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }

            p_player.HandelInputAction(g_event, g_screen, g_sound_bullet);
        }
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen); // xoa man hinh

        g_background.Render(g_screen, NULL);
        Map map_data = game_map.getMap();

        p_player.HandleBullet(g_screen);
        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
        p_player.DoPlayer(map_data);
        p_player.Show(g_screen);

        game_map.SetMap(map_data);
        game_map.DrawMap(g_screen);

        //Draw Geometric
        GeometricFormat rectangle_size(0, 0, SCREEN_WIDTH, 40);
        ColorData color_data(0, 80, 150) ;
        Geometric::RenderRectangle(rectangle_size, color_data, g_screen);

        GeometricFormat outLineSize(1, 1, SCREEN_WIDTH - 1, 38);
        ColorData color_data2(255, 255, 255);
        Geometric::RenderOutline(outLineSize, color_data2, g_screen);


        player_power.Show(g_screen);
        player_money.Show(g_screen);

        for (int i = 0; i < threats_list.size(); i++)
        {
            ThreatsObject* p_threat = threats_list.at(i);
            if (p_threat != NULL)
            {
                p_threat -> SetMapXY(map_data.start_x_, map_data.start_y_);
                p_threat -> ImpMoveType(g_screen);
                p_threat -> DoPlayer(map_data);
                p_threat -> MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
                p_threat -> Show(g_screen);


                SDL_Rect rect_player = p_player.GetRectFrame();

                bool bCol1 = false;
                vector<BulletObject*> tBullet_list = p_threat -> get_bullet_list();
                for (int jj = 0; jj < tBullet_list.size(); ++jj)
                {
                    BulletObject* pt_bullet = tBullet_list.at(jj);
                    if (pt_bullet != NULL)
                    {
                        bCol1 = SDLCommonFunc::CheckCollision(pt_bullet -> GetRect(), rect_player);
                        if (bCol1)
                        {
                            break;
                        }

                    }
                }
                SDL_Rect rect_threat = p_threat -> GetRectFrame();
                bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);
                if (bCol2)
                {
                    int width_exp_frame = exp_main.get_frame_width();
                    int height_exp_height = exp_main.get_frame_height();

                    for (int ex = 0; ex < 4; ex++)
                    {
                        int x_pos = (p_player.GetRect().x + p_player.get_frame_width() * 0.5) - width_exp_frame * 0.5;
                        int y_pos = (p_player.GetRect().y + p_player.get_frame_height() * 0.5) - height_exp_height * 0.5;

                        exp_main.set_frame(ex);
                        exp_main.SetRect(x_pos, y_pos);
                        exp_main.Show(g_screen);
                        SDL_RenderPresent(g_screen);

                    }
                    Mix_PlayChannel(-1, g_sound_exp[0], 0);
                    num_die++;
                    if (num_die <= 3)
                    {
                        p_player.setRect(0, 0);
                        p_player.set_comeback_time(60);
                        SDL_Delay(1000);
                        player_power.Decrease();
                        player_power.Render(g_screen);
                        continue;
                    }
                    else if (MessageBoxW(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
                    {
                        p_threat -> Free();
                        close();
                        SDL_Quit();
                        return 0;
                    }
                }
            }
        }

        int frame_exp_width = exp_threat.get_frame_width();
        int frame_exp_height = exp_threat.get_frame_height();

        vector<BulletObject*> bullet_arr = p_player.get_bullet_list(); //lay ra list dan
        for (int r = 0; r < bullet_arr.size(); ++r)
        {
            BulletObject* p_bullet = bullet_arr.at(r);
            if (p_bullet != NULL)
            {
                for (int t = 0; t < threats_list.size(); ++t)
                {
                    ThreatsObject* obj_threat = threats_list.at(t);
                    if (obj_threat != NULL)
                    {
                        SDL_Rect tRect;
                        tRect.x = obj_threat -> GetRect().x;
                        tRect.y = obj_threat -> GetRect().y;
                        tRect.w = obj_threat -> get_width_frame();
                        tRect.h = obj_threat -> get_height_frame();

                        SDL_Rect bRect = p_bullet -> GetRect();

                        bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);

                        if (bCol)
                        {
                            mark_value++;
                            for (int ex = 0; ex < NUM_FRAME_EXP; ++ex)
                            {
                                int x_pos = p_bullet -> GetRect().x - frame_exp_width * 0.5;     // đặt tâm frame vào vị trí vụ nỏ
                                int y_pos = p_bullet -> GetRect().y - frame_exp_height * 0.5;

                                exp_threat.set_frame(ex);
                                exp_threat.setRect(x_pos, y_pos);
                                exp_threat.Show(g_screen);

                            }
                            p_player.RemoveBullet(r); // xoa vien dan
                            obj_threat -> Free(); // xoa quai vat
                            threats_list.erase(threats_list.begin() + t);
                            Mix_PlayChannel(-1, g_sound_exp[0], 0);
                        }
                    }
                }
            }
        }

        //Show game time
        string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks() / 1000;
        Uint32 val_time = 300 - time_val;
        if (val_time <= 0)
        {
            if (MessageBoxW(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
                {
                    is_quit = true;
                    break;
                }
        }
        else
        {
            string str_val = to_string(val_time);
            str_time += str_val;

            time_game.SetText(str_time);
            time_game.LoadFromRenderText(font_time, g_screen);
            time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
        }

        string val_str_mark = to_string(mark_value);
        string strMark("Mark: ");
        strMark += val_str_mark;

        mark_game.SetText(strMark);
        mark_game.LoadFromRenderText(font_time, g_screen);
        mark_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 50, 15);

        int money_count = p_player.GetMoneyCount();
        string money_str = to_string(money_count);

        money_game.SetText(money_str);
        money_game.LoadFromRenderText(font_time, g_screen);
        money_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 250, 15);

        //Show Boss
        int val = MAX_MAP_X * TILE_SIZE - (map_data.start_x_ + p_player.GetRect().x);
        if (val <= SCREEN_WIDTH) //di den cuoi ban do moi xuat hien boss
        {
            bossObject.SetMapXY(map_data.start_x_, map_data.start_y_);
            bossObject.DoPlayer(map_data);
            bossObject.MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
            bossObject.Show(g_screen);
        }

        SDL_RenderPresent(g_screen); // hien thi cua so len man hinh

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000 / FRAME_PER_SECOND; //fps

        if (real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            if (delay_time >= 0)
                SDL_Delay(delay_time);
        }
    }
    for (int i = 0; i < threats_list.size(); i++)
    {
        ThreatsObject* p_threat = threats_list.at(i);
        if (p_threat)
        {
            p_threat -> Free();
            p_threat = NULL;
        }
    }
    threats_list.clear();
    close();
    return 0;
}
