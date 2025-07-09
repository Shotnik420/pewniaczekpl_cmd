/**
@file main.cpp
*/
#define NCURSES_MOUSE_VERSION
#include <iostream>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <map>
#include <filesystem>
#include <fstream>

#include "./uibuttons/uibuttons.h"
#include "./mecze/mecz.h"
#include "./mecze/team_picker.cpp"
#include "./mecze/team_picker.h"
#include "./player/pilkarz.h"
#include "./user/user.h"
#include "./mecze/kosz.h"
#include "./mecze/pilka.h"

// Deklaracja filesystem
namespace fs = std::filesystem;


// Prototypy funkcji
void draw_art(WINDOW* window, int which,int x, int y );
void draw_logo(WINDOW *win);
std::string password_writing(bool hashed = true);
short get_kurs_color(float kurs1, float kurs2);
void draw_tables(WINDOW *details_win, Mecz &mecz, bool finished);

//Deklaracje zmiennych globalnych
int selected_match_index = -1;
bool user = false;
bool matchOngoing = false;
bool matchOverview = false;
int next_step_speed = 1500;

Mecz *current_match = nullptr;
std::string dane_path = "dane";


int main(int argc, char *argv[]) {
    
    float current_bet = 0.0; // Deklaracja zakladu
    srand(time(NULL)); // Inicjalizacja generatora liczb losowych
    
    int ch = 0; // Zmienna do przechowywania kodu klawisza
    
    initscr(); // Inicjalizacja PDcurses
    cbreak(); // Włączenie trybu cbreak, aby wejście było natychmiastowe
    keypad(stdscr, TRUE); // Włączenie obsługi klawiszy
    noecho(); // Wyłączenie echa klawiszy
    curs_set(0); // Ukrycie kursora
    start_color(); // Inicjalizacja kolorów
    init_ui_colors();  // Inicjalizacja kolorów UI
    mmask_t old; // Zmienna do przechowywania starej maski myszy
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, &old); // Ustawienie maski myszy
    resize_term(40, 100); // Ustawienie rozmiaru terminala


    //Tworzenie głównych okien
    WINDOW *win = newwin(8, 96, 32, 2);
    WINDOW *win2 = newwin(30, 96, 2, 2);
    WINDOW *clicked_win = derwin(win,8, 45, 0, 0);
    WINDOW *details_win = derwin(win2, 30 - 2, 96 - 2, 1, 1);
    WINDOW *art_win = derwin(details_win, 26, 34, 1, 1);

    //Utworzenie okna "pop up" i jego przycisków
    WINDOW *alert_win = newwin(16, 60, 12, 20);
    UIButton alertBackBtn = create_button(5, 10, 10, 3, "Zobacz graczy", 8);
    UIButton alertSaveBtn = create_button(25, 10, 10, 3, "Zapisz stan", 8);
    UIButton alertNextBtn = create_button(45, 10, 10, 3, "Nast mecz", 8);

    //Tablice do przechowywania przycisków
    std::vector<UIButton> buttons;
    std::vector<UIButton> betButtons;

    //Stworzenie przycisków na podstawie folderów w katalogu "dane", oraz ustawianie ich obok siebie.
    int xpos = 3;
    int ypos = 1;
    for (const auto& entry : fs::directory_iterator(dane_path)) {
            if (fs::is_directory(entry)) {
                std::string player_name = entry.path().filename().string();
                buttons.push_back(create_button(xpos, ypos, 18, 5, player_name.c_str(), 8));
                xpos += 20;
                if (xpos > 80){
                    xpos = 3;
                    ypos += 8;
                }
            }
        }
    //Dodanie przycisku utworzenia nowego użytkownika do okna logowania
    UIButton newUserBtn = create_button(70, 23, 14, 5, "  Nowy user  ", 8);
    
    
    //Przygotowanie do wyświetlenia loga Pewniaczek.pl
    werase(win2);
    draw_logo(win2);
    napms(1000); // WYŚWIETLAMY LOGO
    werase(win2);

    //Włączanie obsługi myszy w oknach
    keypad(win, TRUE);
    keypad(win2, TRUE);
    keypad(details_win, TRUE);
    keypad(alert_win, TRUE);
    
    box(win2, 0, 0);    

    


    UIButton toggleBtn = create_button(70, 1, 18, 5, "  Wyniki  ", 102);
    UIButton speedUp = create_button(50, 1, 18, 5, "Przyspiesz", 105);
    UIButton detailsGobackBtn = create_button(50, 1, 18, 5, "  Wroc ", 8);

    //Deklaracja przycisków do zakładów
    UIButton betBtn = create_button(73, 4, 19, 5, "Postaw na druzyne", 8);
    UIButton betBtnRemis = create_button(73, 12, 19, 5, "Postaw na remis", 8);
    UIButton betBtn2 = create_button(73, 20, 19, 5, "Postaw na druzyne", 8);
    UIButton add_50_to_bet = create_button(39, 1, 4, 3, "50", 103);
    UIButton add_100_to_bet = create_button(34, 1, 4, 3, "100", 103);
    UIButton add_500_to_bet = create_button(28, 1, 5, 3, "500", 103);
    UIButton sub_50_from_bet = create_button(39, 4, 4, 3, "-50", 104);
    UIButton sub_100_from_bet = create_button(34, 4, 4, 3, "-100", 104);
    UIButton sub_500_from_bet = create_button(28, 4, 5, 3, "-500", 104);
    
    //Dodanie przycisków do zakładów do wektora
    betButtons.push_back(add_50_to_bet);
    betButtons.push_back(add_100_to_bet);
    betButtons.push_back(add_500_to_bet);
    betButtons.push_back(sub_50_from_bet);
    betButtons.push_back(sub_100_from_bet);
    betButtons.push_back(sub_500_from_bet);
    

    //Utwórz obiekt użytkownika
    User current_user("NULL", "NuLL", 0.0f);


    MEVENT event;
    box(details_win, 0, 0);
    while(user == false) { //loop logowania
        for (UIButton btn : buttons) { // Wyświetlanie przycisków użytkowników
            btn.draw_button(details_win);
        }

        //Wyświetlanie przycisku do tworzenia nowego użytkownika
        newUserBtn.draw_button(win2);

        //Pobieramy dane wejścia
        ch = wgetch(win2);
        //Czy kliknięto mysz?
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                int mx = event.x - win2->_begx;
                int my = event.y - win2->_begy;

                //Dla każdego przycisku ta sama logika 
                for (auto& btn : buttons) {
                    if (btn.is_inside_button(mx, my)) {
                        clear();
                        
                        std::string folder_name = btn.text; // lub btn.label, zależnie od twojej struktury
                        std::string filepath = "./dane/" + folder_name + "/player.txt";
                        std::string haslo;
                        std::ifstream file(filepath);
                        std::string player_name;
                        std::string admin_privileges;
                        float money = 0.0f;
                        bool pass_correct = false;
                        if (!file.is_open()) {
                            mvprintw(2, 1, "Nie mozna otworzyc pliku: %s", filepath.c_str());
                        } else {
                            file >> player_name >>haslo>> money >> admin_privileges;
                        }
                        current_user.setName(folder_name);
                        current_user.setPassword(haslo);
                        current_user.setMoney(money);
                        current_user.setAdmin(stoi(admin_privileges));
                        mvprintw(10, 1, "Podaj haslo: ");
                        refresh();
                        
                        while(!pass_correct){
                            std::string entered_password = password_writing(true);
                            
                            if (entered_password == haslo) {
                                pass_correct = true;
                                mvprintw(1, 1, "Witaj na Pewniaczek.pl %s!", btn.text);
                                user = true;
                            } else {
                                mvprintw(11, 1, "Bledne haslo, sprobuj ponownie.");
                                mvprintw(10, 14, "                    "); // wyczysc wprowadzone znaki
                                refresh();
                            }
                        
                        
                        
                        refresh();
                        }
                        initialize_players(current_user.getName());
                    }
                }
                if (newUserBtn.is_inside_button( mx, my)) {

                    //Logika dodawania nowego użytkownika
                    std::string new_user_name;
                    std::string new_user_password;

                    clear();
                    mvprintw(10, 1, "Podaj imie: ");
                    refresh();
                    while (new_user_name.empty()) {
                     new_user_name= password_writing(false); // false, aby nie maskować znaków
                    }
                    clear();
                    mvprintw(10, 1, "Podaj haslo: ");
                    refresh();
                    while(new_user_password.empty()) {
                        new_user_password = password_writing(false);
                    }
                    current_user.setName(new_user_name);
                    current_user.setPassword(new_user_password);
                    current_user.setMoney(300.0f);
                    user = true;
                    mvprintw(1, 1, "Witaj na Pewniaczek.pl %s!", new_user_name.c_str());
                    refresh();
                    initialize_players(current_user.getName());
                }

            }
        }
        wrefresh(win2);
        
    }
    werase(win2);
    box(win2, 0, 0);


    //
    for (UIButton &btn : buttons) {
        if (btn.text != "admin") {
            btn .draw_button(details_win);
        }
    }  
        
    
    while (current_user.isAdmin()) { //loop admina
        ch = wgetch(win2);
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                int mx = event.x;
                int my = event.y;

                for (auto& btn : buttons) {
                    if (btn.is_inside_button( mx, my)) {

                        clear();

                        std::string folder_name = btn.text;
                        std::string filepath = "./dane/" + folder_name + "/player.txt";
                        int admin = 0;
                        std::ifstream file_in(filepath);
                        if (!file_in.is_open()) {
                            std::cerr << "Nie mozna otworzyc pliku: " << btn.text <<"/player.txt"<< "\n";
                        }
                        
                        std::string imie, haslo;
                        int kasa;
                        
                        file_in >> imie >> haslo >> kasa >> admin;
                        file_in.close();
                        
                        int done = false;

                        
                        while(!done){
                            mvprintw(15, 1, " Dodaj kasy uzytkownikowi %s: ", imie.c_str());
                            refresh();
                            std::string input_str = password_writing(false);
                            try {
                                int liczba = stoi(input_str);
                                done = true; 
                                kasa += liczba;
                                
                                std::ofstream file_out(filepath);
                                if (file_out.is_open()) {
                                    file_out << imie << " " << haslo << " " << kasa << " "<< admin;
                                    file_out.close();
                                    mvprintw(ypos + 1, 1, "Dodano %d do konta %s. Nowy stan: %d", liczba, imie.c_str(), kasa);
                                    refresh();
                                    napms(1000);
                                    werase(win2);
                                    for (UIButton &btn : buttons) {
                                        btn.draw_button(details_win);
                                    } 
                                    refresh();
                                }
                            } catch (...) {
                                mvprintw(ypos + 1, 1, "Blad: wpisz tylko cyfry");

                                refresh();
                                napms(2000);
                            }
                        }
                        
                        refresh();
                        
                    }
                }
            }
        }
        wrefresh(win2);
    } 


    //Usuwamy kontent i dodajemy obramowanie
    werase(win2);
    werase(details_win);
    box(art_win, 0, 0);
    box(clicked_win, 0, 0);
    box(win2,0,0);
    box(clicked_win, 0, 0); 

    //Tworzymy mecz, losowo czy kosz czy pilka
    if(rand() % 2 == 0) {
        current_match = new Kosz(); // Tworzymy nowy mecz koszykówki
    } else {
        current_match = new Pilka(); // Tworzymy nowy mecz piłki nożnej
    }
    
    bool finished = false; // Flaga do sprawdzania, czy mecz jest zakończony

    draw_art(art_win, current_match->getType(), 1, 1); // W zależności od typu meczu, rysujemy odpowiednią grafikę

    

    

    while(ch != 27) { //loop GRY


        for(UIButton &btn : betButtons) {
            // Rysujemy przyciski do zakładów
            btn.draw_button(win);
        }
        toggleBtn.draw_button(win);
        draw_tables(details_win, *current_match, finished);
                        
        betBtn.draw_button(details_win);
        betBtnRemis.draw_button(details_win);
        betBtn2.draw_button(details_win);
        wrefresh(details_win);

        //Refresh wszystkiego
        wrefresh(win);
        wrefresh(win2);
        wrefresh(art_win);
        refresh();

        //Aktualizacja okna bilansu
        mvwprintw(clicked_win, 2, 4, "Saldo: %.2f", current_user.getMoney());
        mvwprintw(clicked_win, 4, 4, "Obecny zaklad: %.2f", current_bet);
        wrefresh(clicked_win);
        napms(40);

        if(finished){ // Wyświetlanie infoboxa po zakończeniu meczu
            box(alert_win, 0, 0);
            mvwprintw(alert_win, 2, 18, "Wygrywa %s", 
            current_match->getWynik1() > current_match->getWynik2() ? 
            current_match->getOpponent1().c_str() : 
            current_match->getOpponent2().c_str());
            int wynik = current_match->getWynik1() - current_match->getWynik2();
            int invested = current_match->getBet1() + current_match->getBet2();
            std::string message = "";
            int payout = current_match->getPayout();
            if (invested - payout > 0) {
                message = "Przegrales " + std::to_string(-invested);
            }else if (invested - payout < 0) {
                message = "Wygrales " + std::to_string(payout);
            }else {
                message = "Nie wygrales, nie straciles";
            }
            
            mvwprintw(alert_win, 4, 18, message.c_str());
            alertBackBtn.draw_button(alert_win);
            alertSaveBtn.draw_button(alert_win);
            alertNextBtn.draw_button(alert_win);
            wrefresh(alert_win);
            
        }
        
        // Obsługa zdarzeń myszy
        ch = wgetch(win);
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                int rel_x = event.x - win->_begx;
                int rel_y = event.y - win->_begy;
               
                
                if (detailsGobackBtn.is_inside_button( rel_x, rel_y)) {
                    if ((event.bstate & BUTTON1_CLICKED)) {
                        selected_match_index = -1;
                        current_match = nullptr;
                        werase(details_win);
                        wrefresh(details_win);
                        wrefresh(win2);
                        wrefresh(win);
                        napms(40);
                    }
                }
                if (toggleBtn.is_inside_button( rel_x, rel_y)) {
                    if (event.bstate & BUTTON1_CLICKED) {
                        if(!matchOverview){
                        matchOngoing = true;
                        werase(art_win);
                        wrefresh(art_win);
                        int line = 3;
                        
                        nodelay(win, TRUE); 
                        for (int i = 0; i < 40; i++) {
                            wattron(art_win, COLOR_PAIR(108));
                            mvwprintw(art_win, 0, 0, "%s %i\n%s %i", 
                                current_match->getOpponent1().c_str(), 
                                current_match->getWynik1(), 
                                current_match->getOpponent2().c_str(),
                                current_match->getWynik2());
                            wattroff(art_win, COLOR_PAIR(108));
                            std::string playText = current_match->get_next_play();
                            int color = current_match->get_pilka_team() == 1 ? 106 : 105;
                            wattron(art_win, COLOR_PAIR(color));
                            mvwprintw(art_win, line, 0, "%s", playText.c_str());
                            wattroff(art_win, COLOR_PAIR(color));
                            line += 3;
                            wrefresh(art_win);
                            
                            if (line >24){
                                werase(art_win);
                                line = 3;
                            }
                            speedUp.draw_button(win);

                            ch = wgetch(win);
                            if(ch == KEY_MOUSE && getmouse(&event) == OK) {
                                rel_x = event.x - win->_begx;
                                rel_y = event.y - win->_begy;
                            
                                if (speedUp.is_inside_button(rel_x, rel_y)) {
                                    if (event.bstate & BUTTON1_CLICKED) {
                                        if (next_step_speed > 200) {
                                            next_step_speed = 200;
                                        }
                                    }
                                }
                            }
                            napms(next_step_speed);
                        }
                        nodelay(win, FALSE); 
                        next_step_speed = 1500;
                        werase(win);
                        box(win, 0, 0);
                        current_match->pilka_state = 0;
                        if (line <24){
                            werase(art_win);
                            line = 3;
                        }else{
                            line +=3;
                        }
                        if(current_match->getWynik1() > current_match->getWynik2()) {

                            mvwprintw(art_win, line, 0, "Wygrywa %s", current_match->getOpponent1().c_str());
                        } else if (current_match->getWynik2() > current_match->getWynik1()) {

                            mvwprintw(art_win, line, 0, "Wygrywa %s", current_match->getOpponent2().c_str());
                        }
                        int invested = current_match->getBet1() + current_match->getBet2() + current_match->getBetRemis();
                        int payout = current_match->getPayout();
                        if (invested - payout < 0) {
                            current_user.setMoney(current_user.getMoney() + payout);
                        }
                        

                        matchOngoing = false;
                        finished = true;
                        }
                        else if(matchOverview) {
                            werase(art_win);
                            werase(win2);
                            werase(details_win);
                            box(win2, 0, 0);
                            box(art_win, 0, 0);
                            matchOverview = false;
                            
                            finished = false;
                            current_bet = 0.0f;
                            current_match = nullptr;
                            wrefresh(details_win);

                            if (rand() % 2 == 0) {
                                static Kosz mecz;
                                current_match = &mecz;
                            } else {
                                static Pilka mecz;
                                current_match = &mecz;
                            }
                        }
                }

                }
                
                for(UIButton &btn : betButtons){
                    if (btn.is_inside_button( rel_x, rel_y)) {
                        if (event.bstate & BUTTON1_CLICKED) {
                            int value = std::stoi(btn.text);
                            if (current_bet + value <= current_user.getMoney()) {
                                current_bet += value;
                            } else {
                                current_bet += current_user.getMoney();
                            }
                            wrefresh(clicked_win);
                            mvwprintw(clicked_win, 2, 4, "Saldo: %.2f", current_user.getMoney());
                            mvwprintw(clicked_win, 4, 4, "Obecny zaklad: %.2f", current_bet);
                            box(clicked_win, 0, 0);
                            wrefresh(clicked_win);
                            napms(40);

                        }
                    }
                }
                
              
                    rel_x = event.x - details_win->_begx;
                    rel_y = event.y - details_win->_begy;
                    if (betBtn.is_inside_button(rel_x, rel_y)) {
                        if (event.bstate & BUTTON1_CLICKED && 
                            current_bet > 0 && current_bet <= current_user.getMoney() && 
                            !finished && !matchOngoing) {
                            if (current_match == nullptr) {
                                werase(details_win);
                                wrefresh(details_win);
                                napms(40);
                                continue;
                            }
                            if (current_match->getBet1() == 0) {
                                current_match->set_bet1(current_bet);
                                current_user.setMoney(current_user.getMoney() - current_bet);
                                wrefresh(clicked_win);
                                mvwprintw(clicked_win, 2, 4, "Saldo: %.2f", current_user.getMoney());
                                mvwprintw(clicked_win, 4, 4, "Obecny zaklad: %.2f", current_bet);
                                box(clicked_win, 0, 0);
                                wrefresh(clicked_win);
                                wrefresh(details_win);
                            }
                            
                            napms(40);
                        }
                    }

                    if (betBtnRemis.is_inside_button(rel_x, rel_y)) {
                        if (event.bstate & BUTTON1_CLICKED && 
                            current_bet > 0 && current_bet <= current_user.getMoney() && 
                            !finished && !matchOngoing) {
                            if (current_match == nullptr) {
                                werase(details_win);
                                wrefresh(details_win);
                                napms(40);
                                continue;
                            }
                            if (current_match->getBetRemis() == 0) {
                                current_match->set_betRemis(current_bet);
                                
                                current_user.setMoney(current_user.getMoney() - current_bet);
                                wrefresh(clicked_win);
                                mvwprintw(clicked_win, 2, 4, "Saldo: %.2f", current_user.getMoney());
                                mvwprintw(clicked_win, 4, 4, "Obecny zaklad: %.2f", current_bet);
                                box(clicked_win, 0, 0);
                                wrefresh(clicked_win);
                                wrefresh(details_win);
                            }
                            
                            napms(40);
                        }
                    }

                    if (betBtn2.is_inside_button(rel_x, rel_y)) {
                    
                    
                        if (event.bstate & BUTTON1_CLICKED
                            && current_bet > 0 && current_bet <= current_user.getMoney() && 
                            !finished && !matchOngoing) {
                            if (current_match == nullptr) {
                                werase(details_win);
                                wrefresh(details_win);
                                napms(40);
                                continue;
                            }
                            if (current_match->getBet2() == 0) {
                                current_match->set_bet2(current_bet);
                                current_user.setMoney(current_user.getMoney() - current_bet);
                                wrefresh(clicked_win);
                                mvwprintw(clicked_win, 2, 4, "Saldo: %.2f", current_user.getMoney());
                                mvwprintw(clicked_win, 4, 4, "Obecny zaklad: %.2f", current_bet);
                                box(clicked_win, 0, 0);
                                wrefresh(clicked_win);
                                wrefresh(details_win);
                            }
                            
                            
                            napms(40);
                        }
                    }
                
            
                if(finished){ // Przyciski alerty
                    rel_x = event.x - alert_win->_begx;
                    rel_y = event.y - alert_win->_begy;
                    if (alertBackBtn.is_inside_button(rel_x, rel_y)) {
                        if (event.bstate & BUTTON1_CLICKED) { // wroc do listy
                            wclear(alert_win);
                            wrefresh(alert_win);

                            finished = false;
                            matchOverview = true;
                            toggleBtn.text = "Nast mecz";

                            draw_tables(details_win, *current_match, finished);
                            wrefresh(details_win);

                            // Overdraw with stdscr if details_win is underneath alert_win
                            overwrite(details_win, stdscr);
                            wrefresh(stdscr);  // force redraw
                            werase(art_win);
                            box(art_win, 0, 0);
                            draw_art(art_win, current_match->getType(), 1, 1);
                            wrefresh(art_win);
                                                    
                        }
                    }
                    if (alertSaveBtn.is_inside_button(rel_x, rel_y)) {
                        if (event.bstate & BUTTON1_CLICKED) { // zapisz stan
                            current_user.saveToFile();
                            save_players(current_user.getName());
                            werase(alert_win);
                            mvwprintw(alert_win, 2, 18, "Stan zapisany!");
                            wrefresh(alert_win);
                            napms(1000);
                        }
                    }
                    if (alertNextBtn.is_inside_button(rel_x, rel_y)) {
                        if (event.bstate & BUTTON1_CLICKED) { // nastepny mecz
                            matchOverview = false;
                            finished = false;
                            current_bet = 0.0f;
                            current_match = nullptr;
                            wclear(alert_win);
                            wrefresh(alert_win); // clears but does NOT remove
                            werase(art_win);
                            werase(win2);
                            werase(details_win);
                            wrefresh(details_win);

                            if (rand() % 2 == 0) {
                                current_match = new Kosz();
                            } else {
                                current_match = new Pilka();
                            }
                            current_match->set_bet1(0);
                            current_match->set_bet2(0);
                            draw_tables(details_win, *current_match, finished);
                            wrefresh(details_win);
                            overwrite(details_win, stdscr);
                            wrefresh(stdscr);  // force redraw
                            box(win2, 0, 0);
                            box(art_win, 0, 0);
                            draw_art(art_win, current_match->getType(), 1, 1);
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

//Funkcja rysująca grafikę w oknie
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
    case 1:
        art = bb;
        lines = 7;
        break;
    case 0:
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


//Funkcja rysująca logo Pewniaczek.pl
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

//Funkcja do zdobywania koloru względem kursu
short get_kurs_color(float kurs1, float kurs2) {
    float kurs = std::max(kurs1, kurs2);
    if (kurs > 2.1f) return 102;        // Fiolet
    else if (kurs > 1.8f) return 101;   // Pomarańcz
    else if (kurs > 1.4f) return 100;   // Niebieski
    else return 1;        // Domyślny (biały)
}
// Funkcja rysująca tabele z danymi meczu
void draw_tables(WINDOW *details_win, Mecz &mecz, bool finished) {
            mvwprintw(details_win, 1, 40, "%i", Player::object_count);

            wattron(details_win, COLOR_PAIR(105));
            mvwprintw(details_win, 2, 40, mecz.getOpponent1().c_str());
            wattroff(details_win, COLOR_PAIR(105));
            mvwprintw(details_win, 3, 40, "---------------------------");
            wattron(details_win, COLOR_PAIR(106));
            mvwprintw(details_win, 18, 40, mecz.getOpponent2().c_str());
            wattroff(details_win, COLOR_PAIR(106));
            mvwprintw(details_win, 19, 40, "---------------------------");

            short color1 = get_kurs_color(mecz.getKurs1(),0.0);
            short color2 = get_kurs_color(mecz.getKurs2(),0.0);
            wattron(details_win, COLOR_PAIR(color1));
            mvwprintw(details_win, 2, 63, "%.2f", mecz.getKurs1());
            wattroff(details_win, COLOR_PAIR(color1));
            wattron(details_win, COLOR_PAIR(color2));
            mvwprintw(details_win, 18, 63, "%.2f", mecz.getKurs2());
            wattroff(details_win, COLOR_PAIR(color2));
            mvwprintw(details_win, 9, 81, "%i", mecz.getBet1());
            mvwprintw(details_win, 17, 81, "%i", mecz.getBetRemis());
            mvwprintw(details_win, 25, 81, "%i", mecz.getBet2());
            
            
            std::vector<float> team1_skill_change = current_match->get_team1_skill_change();
            std::vector<float> team2_skill_change = current_match->get_team2_skill_change();
            for(int i = 0; i < 5; ++i) {
                if (i < mecz.getOpponents1team().size()) {
                    Player* currentPlayer = mecz.getOpponents1team()[i];
                    
                    int colorpair = currentPlayer->get_color();
                    
                    wattron(details_win, COLOR_PAIR(colorpair));
                    mvwprintw(details_win, 4 + i, 40, "%s", currentPlayer->getName().c_str());
                    if(mecz.getType() == 1) {
                        mvwprintw(details_win, 4 + i, 63, "%.2f", currentPlayer->getSkillBasketball());
                    } else {
                        mvwprintw(details_win, 4 + i, 63, "%.2f", currentPlayer->getSkillFootball());
                    }
                    mvwprintw(details_win, 4 + i, 63, "%.2f", currentPlayer->getSkillFootball());
                    wattroff(details_win, COLOR_PAIR(colorpair));
                    if(finished && team1_skill_change[i] != 0.0f){
                        wattron(details_win, COLOR_PAIR(103));
                        mvwprintw(details_win, 4 + i, 67, "+%.2f", team2_skill_change[i]);
                        wattroff(details_win, COLOR_PAIR(103));
                    }


                } else {
                    mvwprintw(details_win, 4 + i, 40, "Brak", i + 1);
                }
                if (i < mecz.getOpponents2team().size()) {
                    auto currentPlayer = mecz.getOpponents2team()[i];
                if (i < mecz.getOpponents2team().size()) {
                    Player* currentPlayer = mecz.getOpponents2team()[i];
                    int colorpair = 0;
                    if (currentPlayer->getType() == 1) {
                        colorpair = 106;
                    } else if (currentPlayer->getType() == 2) {
                        colorpair = 102;
                    }
                    
                    wattron(details_win, COLOR_PAIR(colorpair));
                    mvwprintw(details_win, 20 + i, 40, "%s", currentPlayer->getName().c_str());
                    if(mecz.getType() == 1) {
                        mvwprintw(details_win, 20 + i, 63, "%.2f", currentPlayer->getSkillBasketball());
                    } else {
                        mvwprintw(details_win, 20 + i, 63, "%.2f", currentPlayer->getSkillFootball());
                    }
                    wattroff(details_win, COLOR_PAIR(colorpair));
                    if(finished && team2_skill_change[i] != 0.0f) {
                        wattron(details_win, COLOR_PAIR(103));
                        mvwprintw(details_win, 20 + i, 67, "+%.2f", team2_skill_change[i]);
                        wattroff(details_win, COLOR_PAIR(103));
                    }
                } else {
                    mvwprintw(details_win, 20 + i, 40, "Brak");
                }
            }
        }
    }

// Funkcja do wprowadzania hasła, lub jakiekolwiek innego tekstu
std::string password_writing(bool hashed) {
    echo();
    int c;
    char buffer[256];
    int pos = 0;
    bool pass_correct = false;
    while ((c = getch()) != '\n' && c != 539 && pos < 255) {
            if (c == KEY_BACKSPACE || c == 8) {
                if (pos > 0) {
                    pos--;
                    mvprintw(10, 14 + pos, " ");
                    move(10, 14 + pos);
                }
            } else {
                buffer[pos++] = c;
                if(hashed) mvprintw(10, 14 + pos - 1, "*");  // maskuj znak
            }
            refresh();
    }
    buffer[pos] = '\0';
    std::string entered_password = buffer;
    noecho(); // wyłącz echo, aby nie pokazywać wpisywanego hasła
    return entered_password;
}