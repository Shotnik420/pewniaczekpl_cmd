#define NCURSES_MOUSE_VERSION
#include <iostream>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <map>
#include <filesystem>
#include <fstream>

#include "uibuttons.h"
#include "./mecze/mecz.h"
#include "./mecze/team_picker.cpp"
#include "./bet/bet.h"
#include "./mecze/team_picker.h"
#include "./player/pilkarz.h"
#define WIDTH 60
#define HEIGHT 5
#define ART_HEIGHT 26
#define ART_WIDTH 34
#define SMALL_H 10

namespace fs = std::filesystem;


void draw_art(WINDOW* window, int which,int x, int y );
void draw_status_bar(int money, const char *status);
void draw_logo(WINDOW *win);
void get_window_global_position(WINDOW *win, int &global_y, int &global_x);
short get_border_color(float kurs1, float kurs2);

bool show_details = false;
bool obstawianie = false;
bool IsObstawione = false;
int selected_match_index = -1;
std::vector<std::string> user_list = {};
bool user = false;
std::string current_user = "";

Mecz *current_match = nullptr;
std::string dane_path = "dane";
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
    WINDOW *art_win = derwin(details_win, ART_HEIGHT, ART_WIDTH, 1, 1);
    std::vector<UIButton> buttons;
    int xpos = 3;
    for (const auto& entry : fs::directory_iterator(dane_path)) {
            if (fs::is_directory(entry)) {
                std::string player_name = entry.path().filename().string();
                buttons.push_back(create_button(4, xpos, 18, 5, player_name.c_str(), 8));
                xpos += 8;
            }
        }
    



    werase(win2);
    draw_logo(win2);
    napms(1000); // WYŚWIETLAMY LOGO
    werase(win2);
    box(win2, 2, 0);
 
  
    Mecz nowy_mecz(1);
    mecze[1] = nowy_mecz;
    Bet nowy_bet(1, 0, 0);
    bety[1] = nowy_bet;
    


    wrefresh(win2);
    keypad(win, TRUE);
    keypad(win2, TRUE);
    keypad(details_win, TRUE);
    
    
    box(win2, 0, 0);    

    


    UIButton toggleBtn = create_button(70, 1, 18, 5, "  Wyniki  ", 102);
    UIButton detailsGobackBtn = create_button(50, 1, 18, 5, "  Wroc ", 8);
    UIButton betBtn = create_button(72, 4, 18, 5, "  Bet  ", 8);
    UIButton betBtn2 = create_button(72, 20, 18, 5, "  Bet  ", 8);
    UIButton add_10_to_bet = create_button(39, 1, 4, 3, "10", 103);
    UIButton add_50_to_bet = create_button(34, 1, 4, 3, "50", 103);
    UIButton add_100_to_bet = create_button(28, 1, 5, 3, "100", 103);
    UIButton sub_10_from_bet = create_button(39, 4, 4, 3, "10", 104);
    UIButton sub_50_from_bet = create_button(34, 4, 4, 3, "50", 104);
    UIButton sub_100_from_bet = create_button(28, 4, 5, 3, "100", 104);
        
    
    
    werase(details_win);
    box(details_win, 0, 0);
    

    while(user == false) { //loop logowania

        mvwprintw(details_win, 2, 40, "FUCK");
        for (UIButton btn : buttons) {
            draw_button(details_win, &btn);
        }
        ch = wgetch(win2);
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                int mx = event.x;
                int my = event.y;

                for (auto& btn : buttons) {
                    if (is_inside_button(&btn, mx, my)) {
                        // ✅ Kliknięto przycisk
                        clear();
                        
                        std::string folder_name = btn.text; // lub btn.label, zależnie od twojej struktury
                        std::string filepath = "./dane/" + folder_name + "/player.txt";
                        std::string haslo;
                        std::ifstream file(filepath);
                        std::string player_name;
                        if (!file.is_open()) {
                            mvprintw(2, 1, "Nie mozna otworzyc pliku: %s", filepath.c_str());
                        } else {
                            
                            
                            file >> player_name >>haslo>> money;
                        }
                        
                        mvprintw(10, 1, "Podaj haslo: ");
                        refresh();
                        int c;
                        echo();
                        char buffer[256];
                        int pos = 0;
                        bool pass_correct = false;
                        while(!pass_correct){
                            while ((c = getch()) != '\n' && c != 539 && pos < 255) {
                                if (c == KEY_BACKSPACE || c == 8) {
                                    if (pos > 0) {
                                        pos--;
                                        mvprintw(10, 14 + pos, " ");
                                        move(10, 14 + pos);
                                    }
                                } else {
                                    buffer[pos++] = c;
                                    mvprintw(10, 14 + pos - 1, "*");  // maskuj znak
                                }
                                refresh();
                            }
                            buffer[pos] = '\0';
                            std::string entered_password = buffer;

                            if (entered_password == haslo) {
                                pass_correct = true;
                                mvprintw(1, 1, "Witaj na Pewniaczek.pl %s!", btn.text);
                                user = true;
                                if (player_name == "admin"){
                                    current_user = "admin";
                                }
                            } else {
                                mvprintw(11, 1, "Bledne haslo, sprobuj ponownie.");
                                pos = 0; // resetuj pozycje
                                mvprintw(10, 14, "                    "); // wyczysc wprowadzone znaki
                                refresh();
                            }

                        }
                        
                        refresh();
                        
                    }
                }
            }
        }
        wrefresh(win2);
        
    }
    werase(win2);
    box(win2, 0, 0);
    for (UIButton &btn : buttons) {
        draw_button(details_win, &btn);
    }  
        
    
    while (current_user == "admin") { //loop admina
        ch = wgetch(win2);
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                int mx = event.x;
                int my = event.y;

                for (auto& btn : buttons) {
                    if (is_inside_button(&btn, mx, my)) {

                        clear();

                        std::string folder_name = btn.text;
                        std::string filepath = "./dane/" + folder_name + "/player.txt";

                        std::ifstream file_in(filepath);
                        if (!file_in.is_open()) {
                            std::cerr << "Nie mozna otworzyc pliku: " << btn.text <<"/player.txt"<< "\n";
                        }
                        
                        std::string imie, haslo;
                        int kasa;
                        
                        file_in >> imie >> haslo >> kasa;
                        file_in.close();
                        mvprintw(15, 1, " Dodaj kasy uzytkownikowi %s: ", imie.c_str());
                        refresh();
                        int c;
                        noecho();
                        echo();
                        char buffer[256];
                        int pos = 0;
                        bool pass_correct = false;
                        int ypos = 16;
                        bool done = false;
                        while(!done){
                            while ((c = getch()) != '\n' && c != 539 && pos < 255) {
                                if (c == KEY_BACKSPACE || c == 8) {
                                    if (pos > 0) {
                                        pos--;
                                        mvprintw(ypos, 14 + pos, " ");
                                        move(ypos, 14 + pos);
                                    }
                                } else {
                                    buffer[pos++] = c;
                                }
                                refresh();
                            }
                            buffer[pos] = '\0';
                            std::string input_str = buffer;
                            int liczba = 0;
                            try {
                                liczba = std::stoi(input_str);
                                done = true; 
                                kasa += liczba;
                                std::ofstream file_out(filepath);
                                if (file_out.is_open()) {
                                    file_out << imie << " " << haslo << " " << kasa << "\n";
                                    file_out.close();
                                    mvprintw(ypos + 1, 1, "Dodano %d do konta %s. Nowy stan: %d", liczba, imie.c_str(), kasa);
                                    refresh();
                                    napms(2000);
                                    werase(win2);
                                    for (UIButton &btn : buttons) {
                                        draw_button(details_win, &btn);
                                    } 
                                    refresh();
                                }
                            } catch (...) {
                                mvprintw(ypos + 1, 1, "Blad: wpisz tylko cyfry");
                                pos = 0;
                                memset(buffer, 0, sizeof(buffer));
                                mvprintw(ypos, 14, std::string(40, ' ').c_str()); // wyczyść pole
                                move(ypos, 14);
                            }
                        }
                        
                        refresh();
                        
                    }
                }
            }
        }
        wrefresh(win2);
    } 


    box(art_win, 0, 0);
    box(clicked_win, 0, 0);
    
    draw_button(win, &toggleBtn);
    draw_button(win, &add_10_to_bet);
    draw_button(win, &add_50_to_bet);
    draw_button(win, &add_100_to_bet);
    draw_button(win, &sub_10_from_bet);
    draw_button(win, &sub_50_from_bet);
    draw_button(win, &sub_100_from_bet);
    wrefresh(win);
    while(ch != 27) { //loop GRY
        if (!show_details) {
            draw_button(win, &detailsGobackBtn);
            
            
            
            Mecz &mecz = nowy_mecz;
            current_match = &mecz;
            draw_art(art_win, mecz.getType(), 1, 1);
            
            wattr_on(details_win, A_BOLD, nullptr);
            mvwprintw(details_win, 2, 40, mecz.getOpponent1().c_str());
            mvwprintw(details_win, 3, 40, "---------------------------");
            mvwprintw(details_win, 18, 40, mecz.getOpponent2().c_str());
            mvwprintw(details_win, 19, 40, "---------------------------");
            wattr_off(details_win, A_BOLD,nullptr);
            short color1 = get_border_color(mecz.getKurs1(),0.0);
            short color2 = get_border_color(mecz.getKurs2(),0.0);
            wattron(details_win, COLOR_PAIR(color1));
            mvwprintw(details_win, 2, 63, "%.2f", mecz.getKurs1());
            wattroff(details_win, COLOR_PAIR(color1));
            wattron(details_win, COLOR_PAIR(color2));
            mvwprintw(details_win, 18, 63, "%.2f", mecz.getKurs2());
            wattroff(details_win, COLOR_PAIR(color2));
            mvwprintw(details_win, 9, 80, "%i", mecz.getBet1());
            mvwprintw(details_win, 25, 80, "%i", mecz.getBet2());
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
                    mvwprintw(details_win, 4 + i, 40, "%s", currentPlayer.getName().c_str());
                    mvwprintw(details_win, 4 + i, 63, "%.2f", currentPlayer.getSkillFootball());
                    wattroff(details_win, COLOR_PAIR(colorpair));


                } else {
                    mvwprintw(details_win, 4 + i, 40, "Brak", i + 1);
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
                    mvwprintw(details_win, 20 + i, 40, mecz.getOpponents2team()[i].getName().c_str());
                    mvwprintw(details_win, 20 + i, 63, "%.2f", currentPlayer.getSkillFootball());
                    wattroff(details_win, COLOR_PAIR(colorpair));
                } else {
                    mvwprintw(details_win, 20 + i, 40, "Brak");
                }
            }
            
            draw_button(details_win, &betBtn);
            draw_button(details_win, &betBtn2);
            wrefresh(details_win);
            
           
        
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
                    if ((event.bstate & BUTTON1_CLICKED) && show_details) {
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
                
                if (!show_details) {
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
            

            }
        
        }
        
        napms(10);
    }
    
    delwin(win);
    endwin();
    return 0;
}


void draw_art(WINDOW* window, int which,int x, int y ) {
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

    const char **art;
    int lines = 0;

    switch (which)
    {
    case 0:
        art = bb;
        lines = 7;
        break;
    case 1:
        art = fb;
        lines = 6;
        break;
    default:
        art = fb;
    }

    for (int i = 0; i<lines; ++i) {
        mvwprintw(window, y + i, x, "%s", art[i]);
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


