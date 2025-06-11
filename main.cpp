#define NCURSES_MOUSE_VERSION
#include <iostream>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <map>

#include "uibuttons.h"
#include "./mecze/mecz.h"
#include "./mecze/team_picker.cpp"
#include "./bet/bet.h"
#include "./mecze/team_picker.h"
#include "./player/pilkarz.h"
#define WIDTH 60
#define HEIGHT 5
#define ART_HEIGHT 8
#define SMALL_W 30
#define SMALL_H 10
void draw_art_in_smalls(WINDOW *small_wins[], int count, std::map<int, Mecz> mecze);
void draw_status_bar(int money, const char *status);
void draw_logo(WINDOW *win);
void get_window_global_position(WINDOW *win, int &global_y, int &global_x);
short get_border_color(float kurs1, float kurs2);

bool show_details = false;
bool obstawianie = false;
bool IsObstawione = false;
int selected_match_index = -1;

Mecz *current_match = nullptr;

int main(int argc, char *argv[]) {
    initialize_players();
    float current_bet = 0.0;
    std::map<int, Mecz> mecze;
    std::map<int, Bet> bety;
    srand(time(NULL));
    MEVENT event;
    int ch = 0;
    float money = 500;
    const char *status = "Normalny";
    bool show_ascii = false;
    bool button_focused = false;
    int NUM_SMALL_WINDOWS = 9;
    int SMALL_WIN_WIDTH = 28;
    int SMALL_WIN_HEIGHT = 9;
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();
    init_ui_colors(); 
    
    mmask_t old;
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, &old);
    resize_term(40, 100);
    // Utwórz główne okno
    int start_y = 32; // poniżej status bara
    int start_x = 2;

    int win2_height = 30; 
    int win2_width = 96; 
    WINDOW *win = newwin(8, 96, start_y, start_x);
    WINDOW *win2 = newwin(win2_height, win2_width, 2, 2);
    WINDOW *clicked_win = derwin(win,8, 45, 0, 0);

    WINDOW *details_win = derwin(win2, win2_height - 2, win2_width - 2, 1, 1);
    WINDOW *small_wins[NUM_SMALL_WINDOWS];
    draw_logo(win2);
    napms(1000); // WYŚWIETLAMY LOGO
    werase(win2);
    box(win2, 2, 0);
 
    int cols = 3;
    int rows = 3;
    
    int padding_x = (win2_width - cols * SMALL_WIN_WIDTH) / (cols + 1);
    int padding_y = (win2_height - rows * SMALL_WIN_HEIGHT) / (rows + 1);

    // Pozycja pierwszego okna względem win2
    int small_start_x = 2 + padding_x;
    int small_start_y = 2 + padding_y;

    // Rozmiar każdego małego okienka
    int win_w = SMALL_WIN_WIDTH;
    int win_h = SMALL_WIN_HEIGHT;

    // Tworzenie i rysowanie małych okien
    for (int i = 0; i < NUM_SMALL_WINDOWS; ++i) {
        Mecz nowy_mecz(i);
        mecze[i] = nowy_mecz;
        Bet nowy_bet(i, 0, 0);
        bety[i] = nowy_bet;
    }


    


    wrefresh(win2);
    keypad(win, TRUE);
    keypad(win2, TRUE);
    keypad(details_win, TRUE);
    
    ch = wgetch(win);


     box(win2, 0, 0);
        
    UIButton toggleBtn = create_button(70, 1, 18, 5, "  Wyniki  ", 102);
    UIButton detailsGobackBtn = create_button(50, 1, 18, 5, "  Wroc ", 8);
    UIButton betBtn = create_button(60, 3, 18, 5, "  Bet  ", 8);
    UIButton betBtn2 = create_button(60, 20, 18, 5, "  Bet  ", 8);
    UIButton add_10_to_bet = create_button(39, 1, 4, 3, "10", 103);
    UIButton add_50_to_bet = create_button(34, 1, 4, 3, "50", 103);
    UIButton add_100_to_bet = create_button(28, 1, 5, 3, "100", 103);
    UIButton sub_10_from_bet = create_button(39, 4, 4, 3, "10", 104);
    UIButton sub_50_from_bet = create_button(34, 4, 4, 3, "50", 104);
    UIButton sub_100_from_bet = create_button(28, 4, 5, 3, "100", 104);
        
    draw_button(win, &toggleBtn);
    draw_button(win, &add_10_to_bet);
    draw_button(win, &add_50_to_bet);
    draw_button(win, &add_100_to_bet);
    draw_button(win, &sub_10_from_bet);
    draw_button(win, &sub_50_from_bet);
    draw_button(win, &sub_100_from_bet);

    while(ch != 27) { //loop
        if (show_details) {
            draw_button(win, &detailsGobackBtn);
            
            wrefresh(win);
            werase(details_win);
            box(details_win, 0, 0);
            Mecz &mecz = mecze[selected_match_index];
            current_match = &mecz;
            mvwprintw(details_win, 8, 6, mecz.getOpponent1().c_str());
            mvwprintw(details_win, 18, 6, mecz.getOpponent2().c_str());
            short color1 = get_border_color(mecz.getKurs1(),0.0);
            short color2 = get_border_color(mecz.getKurs2(),0.0);
            wattron(details_win, COLOR_PAIR(color1));
            mvwprintw(details_win, 11, 60, "|%.2f", mecz.getKurs1());
            wattroff(details_win, COLOR_PAIR(color1));
            wattron(details_win, COLOR_PAIR(color2));
            mvwprintw(details_win, 15, 60, "|%.2f", mecz.getKurs2());
            wattroff(details_win, COLOR_PAIR(color2));
            mvwprintw(details_win, 11, 80, "|%i", mecz.getBet1());
            mvwprintw(details_win, 15, 80, "|%i", mecz.getBet2());
            for(int i = 0; i < 5; ++i) {
                if (i < mecz.getOpponents1team().size()) {
                    auto currentPlayer = mecz.getOpponents1team()[i];
                    int colorpair =0;
                    if (currentPlayer.getType()==1){
                        currentPlayer = Pilkarz(currentPlayer.getPID(), currentPlayer.getName(), currentPlayer.getSkillFootball(), currentPlayer.getSkillBasketball(), currentPlayer.getSkillTennis(), currentPlayer.getConcussionDayLeft());
                        colorpair = 106;
                    }else if (currentPlayer.getType()==2){
                        currentPlayer = Koszykarz(currentPlayer.getPID(), currentPlayer.getName(), currentPlayer.getSkillFootball(), currentPlayer.getSkillBasketball(), currentPlayer.getSkillTennis(), currentPlayer.getConcussionDayLeft());
                        colorpair = 102;
                    }
                    
                    wattron(details_win, COLOR_PAIR(colorpair));
                    mvwprintw(details_win, 4 + i, 30, "%s", currentPlayer.getName().c_str());
                    mvwprintw(details_win, 4 + i, 50, "%.2f", currentPlayer.getSkillFootball());
                    wattroff(details_win, COLOR_PAIR(colorpair));


                } else {
                    mvwprintw(details_win, 4 + i, 30, "Brak", i + 1);
                }
                if (i < mecz.getOpponents2team().size()) {
                    auto currentPlayer = mecz.getOpponents2team()[i];
                    int colorpair =0;
                    if (currentPlayer.getType()==1){
                        currentPlayer = Pilkarz(currentPlayer.getPID(), currentPlayer.getName(), currentPlayer.getSkillFootball(), currentPlayer.getSkillBasketball(), currentPlayer.getSkillTennis(), currentPlayer.getConcussionDayLeft());
                        colorpair = 106;
                    }else if (currentPlayer.getType()==2){
                        currentPlayer = Koszykarz(currentPlayer.getPID(), currentPlayer.getName(), currentPlayer.getSkillFootball(), currentPlayer.getSkillBasketball(), currentPlayer.getSkillTennis(), currentPlayer.getConcussionDayLeft());
                        colorpair = 102;
                    }
                    
                    wattron(details_win, COLOR_PAIR(colorpair));
                    mvwprintw(details_win, 18 + i, 30, mecz.getOpponents2team()[i].getName().c_str());
                    wattroff(details_win, COLOR_PAIR(colorpair));
                } else {
                    mvwprintw(details_win, 18 + i, 30, "Brak");
                }
            }
            
            draw_button(details_win, &betBtn);
            draw_button(details_win, &betBtn2);
            wrefresh(details_win);
            ch = wgetch(details_win);
           
        
        } else {
            for (int i = 0; i < NUM_SMALL_WINDOWS; ++i) {
                
                int row = i / cols;
                int col = i % cols;

                int win_y = 1 + padding_y + row * (SMALL_WIN_HEIGHT + padding_y);
                int win_x = 1 + padding_x + col * (SMALL_WIN_WIDTH + padding_x);
                

                small_wins[i] = derwin(win2, SMALL_WIN_HEIGHT, SMALL_WIN_WIDTH, win_y, win_x);
                short color = get_border_color(mecze[i].getKurs1(), mecze[i].getKurs2());
                float payout1 = 0.0;
                float payout2 = 0.0;
                if(obstawianie or IsObstawione){
                    Mecz &current_mecz = mecze[i];
                    if (current_mecz.getWynik() != 0) {
                        float invested = current_mecz.getBet1() + current_mecz.getBet2();
                        payout1 = current_mecz.getPayout1();
                        payout2 = current_mecz.getPayout2();
                        money += payout1 + payout2;
                        if (payout1 > invested || payout2 > invested) {
                            color = 103; 
                        } else if (payout1 <= invested && payout2 <= invested) {
                            color = 104; // czerwony
                        } 
                        
                    } 
                }else{
                    short color = get_border_color(mecze[i].getKurs1(), mecze[i].getKurs2());
                }
                
                wattron(small_wins[i], COLOR_PAIR(color)); 
                box(small_wins[i], 0, 0);
                if(obstawianie or IsObstawione){

                    mvwprintw(small_wins[i], 3, 3, " %.2f ", payout1 + payout2);
                }
                wattroff(small_wins[i], COLOR_PAIR(color));
                
                mvwprintw(small_wins[i], 0, 2, " Mecz %d ", i + 1);
                wrefresh(small_wins[i]);
                if(obstawianie){

                    napms(800);
                }
            }
            
            if (!IsObstawione and !obstawianie){
                draw_art_in_smalls(small_wins, NUM_SMALL_WINDOWS, mecze);
            }
            if (obstawianie) {
                IsObstawione = true;
                obstawianie = false;
            }

        }

        wrefresh(win);
        wrefresh(win2);
        refresh();
        mvwprintw(clicked_win, 2, 4, "Saldo: %.2f", money);
        mvwprintw(clicked_win, 4, 4, "Obecny zaklad: %.2f", current_bet);
        box(clicked_win, 0, 0);
        wrefresh(clicked_win);
        napms(40);
        ch = wgetch(win);
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                int rel_x = event.x - start_x;
                int rel_y = event.y - start_y;
               
               
                if (is_inside_button(&detailsGobackBtn, rel_x, rel_y)) {
                    if (event.bstate & BUTTON1_CLICKED) {
                        show_details = false;
                        selected_match_index = -1;
                        current_match = nullptr;
                        werase(details_win);
                        wrefresh(details_win);
                        wrefresh(win2);
                        wrefresh(win);
                        napms(40);
                    }
                }
                if (is_inside_button(&toggleBtn, rel_x, rel_y)) {
                    if (event.bstate & BUTTON1_CLICKED) {
                        if(!obstawianie) {
                            if(IsObstawione){
                                IsObstawione = false;
                                for (int i = 0; i < NUM_SMALL_WINDOWS; ++i) {
                                    Mecz nowy_mecz(i);
                                    mecze[i] = nowy_mecz;
                                    Bet nowy_bet(i, 0, 0);
                                    bety[i] = nowy_bet;
                                }
                            }
                            else{
                                show_details = false;
                                obstawianie = true;
                            }
                            
                            selected_match_index = -1;
                            werase(details_win);
                            wrefresh(details_win);
                            wrefresh(win2);
                            wrefresh(win);
                            napms(40);
                        }
                    }
                }
                if (is_inside_button(&add_10_to_bet, rel_x, rel_y)) {
                    if (event.bstate & BUTTON1_CLICKED) {
                        if (current_bet + 10.0 <= money) {
                            current_bet += 10.0;
                        } else {
                            current_bet += money;
                        }
                        wrefresh(clicked_win);
                        mvwprintw(clicked_win, 2, 4, "Saldo: %.2f", money);
                        mvwprintw(clicked_win, 4, 4, "Obecny zaklad: %.2f", current_bet);
                        box(clicked_win, 0, 0);
                        wrefresh(clicked_win);
                        napms(40);
                    }
                }
                if (is_inside_button(&add_50_to_bet, rel_x, rel_y)) {
                    if (event.bstate & BUTTON1_CLICKED) {
                        if (current_bet + 50.0 <= money) {
                            current_bet += 50.0;
                        } else {
                            current_bet += money;
                        }
                        wrefresh(clicked_win);
                        mvwprintw(clicked_win, 2, 4, "Saldo: %.2f", money);
                        mvwprintw(clicked_win, 4, 4, "Obecny zaklad: %.2f", current_bet);
                        box(clicked_win, 0, 0);
                        wrefresh(clicked_win);
                        napms(40);
                    }
                }
                if (is_inside_button(&add_100_to_bet, rel_x, rel_y)) {
                    if (event.bstate & BUTTON1_CLICKED) {
                        if (current_bet + 100.0 <= money) {
                            current_bet += 100.0;
                        } else {
                            current_bet += money;
                        }
                        wrefresh(clicked_win);
                        mvwprintw(clicked_win, 2, 4, "Saldo: %.2f", money);
                        mvwprintw(clicked_win, 4, 4, "Obecny zaklad: %.2f", current_bet);
                        box(clicked_win, 0, 0);
                        wrefresh(clicked_win);
                        napms(40);
                    }
                }
                if (is_inside_button(&sub_100_from_bet, rel_x, rel_y)) {
                    if (event.bstate & BUTTON1_CLICKED) {
                        if (current_bet >= 100.0) {
                            current_bet -= 100.0;
                        } else {
                            current_bet = 0.0;
                        }
                        wrefresh(clicked_win);
                        mvwprintw(clicked_win, 2, 4, "Saldo: %.2f", money);
                        mvwprintw(clicked_win, 4, 4, "Obecny zaklad: %.2f", current_bet);
                        box(clicked_win, 0, 0);
                        wrefresh(clicked_win);
                        napms(40);
                    }
                }
                if (is_inside_button(&sub_50_from_bet, rel_x, rel_y)) {
                    if (event.bstate & BUTTON1_CLICKED) {
                        if (current_bet >= 50.0) {
                            current_bet -= 50.0;
                        } else {
                            current_bet = 0.0;
                        }
                        wrefresh(clicked_win);
                        mvwprintw(clicked_win, 2, 4, "Saldo: %.2f", money);
                        mvwprintw(clicked_win, 4, 4, "Obecny zaklad: %.2f", current_bet);
                        box(clicked_win, 0, 0);
                        wrefresh(clicked_win);
                        napms(40);
                    }
                }
                if (is_inside_button(&sub_10_from_bet, rel_x, rel_y)) {
                    if (event.bstate & BUTTON1_CLICKED) {
                        if (current_bet >= 10.0) {
                            current_bet -= 10.0;
                        } else {
                            current_bet = 0.0;
                        }
                        wrefresh(clicked_win);
                        mvwprintw(clicked_win, 2, 4, "Saldo: %.2f", money);
                        mvwprintw(clicked_win, 4, 4, "Obecny zaklad: %.2f", current_bet);
                        box(clicked_win, 0, 0);
                        wrefresh(clicked_win);
                        napms(40);
                    }
                }
                
                if (show_details) {
                    rel_x = event.x;
                    rel_y = event.y;
                    if (is_inside_button(&betBtn, rel_x, rel_y)) {
                        if (event.bstate & BUTTON1_CLICKED && current_bet > 0 && current_bet <= money) {
                            if (current_match == nullptr) {
                                werase(details_win);
                                wrefresh(details_win);
                                napms(40);
                                continue;
                            }
                            if (current_match->getBet1() == 0) {
                                current_match->set_bet1(current_bet);
                                money -= current_bet;
                                wrefresh(clicked_win);
                                mvwprintw(clicked_win, 2, 4, "Saldo: %.2f", money);
                                mvwprintw(clicked_win, 4, 4, "Obecny zaklad: %.2f", current_bet);
                                box(clicked_win, 0, 0);
                                wrefresh(clicked_win);
                                wrefresh(details_win);
                            }
                            
                            napms(40);
                        }
                    }
                    if (is_inside_button(&betBtn2, rel_x, rel_y)) {
                    
                    
                        if (event.bstate & BUTTON1_CLICKED&& current_bet > 0 && current_bet <= money) {
                            if (current_match == nullptr) {
                                werase(details_win);
                                wrefresh(details_win);
                                napms(40);
                                continue;
                            }
                            if (current_match->getBet2() == 0) {
                                current_match->set_bet2(current_bet);
                                money -= current_bet;
                                wrefresh(clicked_win);
                                mvwprintw(clicked_win, 2, 4, "Saldo: %.2f", money);
                                mvwprintw(clicked_win, 4, 4, "Obecny zaklad: %.2f", current_bet);
                                box(clicked_win, 0, 0);
                                wrefresh(clicked_win);
                                wrefresh(details_win);
                            }
                            
                            
                            napms(40);
                        }
                    }
                }
            

                for (int i = 0; i < 9; i++) {
                    if(show_details) {
                        break;
                    }
                    int win_y = small_start_y + (i / cols) * (win_h + padding_y);
                    int win_x = small_start_x + (i % cols) * (win_w + padding_x);

                    if (event.y >= win_y && event.y < win_y + win_h &&
                        event.x >= win_x && event.x < win_x + win_w) {
                        
                        
                        
                        show_details = true;
                        selected_match_index = i;
                        
                        wrefresh(win2);
                        break;
                    }
                }
            }
        
        }
        
        napms(10);
    }
    
    delwin(win);
    endwin();
    return 0;
}


void draw_art_in_smalls(WINDOW *small_wins[], int count, std::map<int, Mecz> mecze) {
    const char *bb[] = {
        "|__  o\\",
        "| W    \\O",
        "|       |\\_         |\\",
        "|      /-\\           \\O",
        "|    /     \\          |",
        "|                    /|",
        "|                   |  \\"
    };

    const char *fb[] = {
        " ",
        "",
        "                  ___",
        " o__        o__  |   |\\",
        "/|          /\\   |   |X\\",
        "/ > o        <\\  |   |XX\\"
        " "
        
    };

    for (int i = 0; i < count; i++) {

        const char **art = (mecze[i].getType() == 0) ? bb : fb;
        int lines = (mecze[i].getType() == 0) ? sizeof(bb) / sizeof(bb[0]) : sizeof(fb) / sizeof(fb[0]);

        for (int y = 0; y < lines && y + 1 < 9; y++) {
            mvwprintw(small_wins[i], 1 + y, 1, "%s", art[y]);
        }

        wrefresh(small_wins[i]);
    }
}

void draw_status_bar(int money, const char *status) {
    time_t now;
    time(&now);
    char *time_str = ctime(&now);
    time_str[strlen(time_str)-1] = '\0';
    
    attron(A_REVERSE);
    mvprintw(0, 0, "Pieniądze: %d | Status: %s | Czas: %s", money, status, time_str);
    clrtoeol();
    attroff(A_REVERSE);
}

void draw_logo(WINDOW *win) {
    const char *logo[] = {
        " (                                                        (    (     ",
        " )\\ )                                                 )   )\\ ) )\\ )  ",
        "(()/(   (   (  (          (      )             (   ( /(  (()/((()/(  ",
        " /(_)) ))\\  )\\))(    (    )\\  ( /(   (   (    ))\\  )\\())  /(_))/(_)) ",
        "(_))  /((_)((_)()\\   )\\ )((_) )(_))  )\\  )\\  /((_)((_)/  (_)) (_))   ",
        "| _ \\(_))  _(()((_) _(_/( (_)((_)_  ((_)((_)(_))  | |(_) | _ \\| |    ",
        "|  _// -_) \\ V  V /| ' \\))| |/ _` |/ _| |_ // -_) | / /  |  _/| |__  ",
        "|_|  \\___|  \\_/\\_/ |_||_| |_|\\__,_|\\__| /__/\\___| |_\\_\\  |_|  |____|"
    };
    int lines = sizeof(logo) / sizeof(logo[0]);

    int win_h, win_w;
    getmaxyx(win, win_h, win_w);

    werase(win);
    box(win, 0, 0);

    for (int i = 0; i < lines; i++) {
        int x = (win_w - strlen(logo[i])) / 2;
        int y = (win_h - lines) / 2 + i;
        mvwprintw(win, y, x, "%s", logo[i]);
    }

    wrefresh(win);
}

void get_window_global_position(WINDOW *win, int &global_y, int &global_x) {
    global_y = 0;
    global_x = 0;

    int wy, wx;
    getbegyx(win, wy, wx);
    global_y = wy;
    global_x = wx;
}

short get_border_color(float kurs1, float kurs2) {
    float kurs = std::max(kurs1, kurs2);
    if (kurs > 2.1f) return 102;        // Fiolet
    else if (kurs > 1.8f) return 101;   // Pomarańcz
    else if (kurs > 1.4f) return 100;   // Niebieski
    else return 1;        // Domyślny (biały)
}


