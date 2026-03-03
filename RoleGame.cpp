#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;

struct character
{
    string name = "Unkown";
    int health = 50;
    int armor = 0;
    int damage = 15;
    int x = 0;
    int y = 0;
};

struct player_star_power 
{
    bool soul_strength = false; // после 3 игр
    bool furious_hero = false;  // после 6 игр
    bool last_chanсe = false;  // после 9 игр
    bool living_steel = false; // после 12 игр
};

void input_play_place (char (&place)[20][20], int &score, int &kills, vector <character> &vec_character) 
{
    cout << endl;
    cout << "=====================================" << endl;
    cout << "            ||PLAY PLACE||           " << endl;
    cout << "=====================================" << endl;
    for (int i = 0; i < 19; i++) 
    {
        for (int j = 0; j < 19; j++) 
        {
            cout << place[i][j] << " ";
        }
        cout << endl;
    }
    cout << "=====================================" << endl;
    cout << "HEALTH: " << vec_character[0].health << "                 ARMOR: " << vec_character[0].armor << endl;
    cout << "SCORE: " << score << endl;
    cout << "KILLS: " << kills << endl;
}

void damage_person(vector<character>& vec_character, int itr, bool (&place_fulled)[20][20], char (&place)[20][20], int &score, int &kills, player_star_power &star, int &hit_streak) 
{
    int player_damage = vec_character[0].damage;

    if (star.furious_hero) // обилка берсерка
    {
        player_damage += hit_streak;
        if (hit_streak > 0) 
        {
            cout << "=*= Damage Boost on " << hit_streak << " ! =*=" << endl;
        }
    }
    
    // Сохраняем старые координаты игрока
    int old_player_x = vec_character[0].x;
    int old_player_y = vec_character[0].y;
    
    // Сохраняем координаты врага
    int enemy_x = vec_character[itr].x;
    int enemy_y = vec_character[itr].y;
    
    // Логика урона
    if (vec_character[itr].armor > 0) 
    {
        vec_character[itr].armor -= player_damage;
        
        if (vec_character[itr].armor < 0) 
        {
            // Броня пробита, остаток урона идет на здоровье
            int leftover_damage = -vec_character[itr].armor;
            cout << "Armor break! " << leftover_damage << " damage goes to health" << endl;
            
            vec_character[itr].armor = 0;
            vec_character[itr].health -= leftover_damage;
        }
        else 
        {
            cout << player_damage << " damage to armor" << endl;
            cout << "Armor left: " << vec_character[itr].armor << endl;
            return; // Враг жив, выходим
        }
    }
    else 
    {
        // Нет брони - весь урон на здоровье
        cout << player_damage << " damage to health" << endl;
        vec_character[itr].health -= player_damage;
    }
    
    // Проверяем, умер ли враг
    if (vec_character[itr].health <= 0) 
    {
        cout << vec_character[itr].name << " has been killed!" << endl;
        
        if (star.soul_strength) // Активация звездной силы
        {
            vec_character[0].health += 30;
            cout << "=*= For killing an enemy you get 30 health points! =*=" << endl;
            if (vec_character[0].health > 150) vec_character[0].health = 150;
        }

        // 1. Очищаем клетку врага
        place[enemy_x][enemy_y] = '.';
        place_fulled[enemy_x][enemy_y] = false;
        
        // 2. Очищаем старую позицию игрока
        place[old_player_x][old_player_y] = '.';
        place_fulled[old_player_x][old_player_y] = false;
        
        // 3. Обновляем координаты игрока
        vec_character[0].x = enemy_x;
        vec_character[0].y = enemy_y;
        
        // 4. Ставим игрока на место убитого врага
        place[enemy_x][enemy_y] = 'P';
        place_fulled[enemy_x][enemy_y] = true;
        
        // 5. Помечаем врага как мертвого
        vec_character[itr].name = "DEAD";
        vec_character[itr].health = 0;
        score += 2;
        kills++;
    }
    else 
    {
        cout << "Enemy health left: " << vec_character[itr].health << endl;
        score++;
    }

    if (star.living_steel) 
    {
        cout << "=*= Armor is restored by 5 points =*= " << endl;
        vec_character[0].armor += 5;
        if (vec_character[0].armor > 50) vec_character[0].armor = 50;
    }
}

int find_iterator_enemy (vector <character> &vec_character, int position_x, int position_y)
{
    for(int i = 1; i <= 9; i++) 
    {
        if (vec_character[i].x == position_x && vec_character[i].y == position_y) 
        {
            if(vec_character[i].name == "DEAD") continue;            
            else return i;
        }
    }
    return 0;
}

void player_movement (char (&place)[20][20], bool (&place_fulled)[20][20], vector <character> &vec_character, char &action, int &score, int &kills, player_star_power &star) 
{
    int temp_x, temp_y; 
    int player_position_x, player_position_y;
    int hit_streak_damage = 0; // Временные данные

    // Движение игрока

    if (action == 'L') // Для движения влево
    {
        if (vec_character[0].y-1 < 0) { return; }
        else 
        {
            temp_x = vec_character[0].x;
            temp_y = vec_character[0].y;
            if (place_fulled[vec_character[0].x][vec_character[0].y - 1] == true) 
            {
                player_position_x = vec_character[0].x;
                player_position_y = vec_character[0].y - 1;
                cout << "Find enemy!" << endl;
                int itr = find_iterator_enemy(vec_character, player_position_x, player_position_y);
                damage_person(vec_character, itr, place_fulled, place, score, kills, star, hit_streak_damage);
                hit_streak_damage += 15;
            } 
            else 
            {
                vec_character[0].y = vec_character[0].y - 1;
                cout << "Player movement on the left" << endl;
                place[vec_character[0].x][vec_character[0].y] = 'P';
                place_fulled[vec_character[0].x][vec_character[0].y] = true;
                place_fulled[temp_x][temp_y] = false;
                place[temp_x][temp_y] = '.';
                hit_streak_damage = 0;
            }
        }
    }
    else if (action == 'R') // Движение направо
    {
        if (vec_character[0].y + 1 >= 19) { return; }
        else 
        {
            temp_x = vec_character[0].x;
            temp_y = vec_character[0].y;
            if (place_fulled[vec_character[0].x][vec_character[0].y + 1] == true) 
            {
                player_position_x = vec_character[0].x;
                player_position_y = vec_character[0].y + 1;
                cout << "Find enemy!" << endl;
                int itr = find_iterator_enemy(vec_character, player_position_x, player_position_y);
                damage_person(vec_character, itr, place_fulled, place, score, kills, star, hit_streak_damage);
                hit_streak_damage += 15;
            } 
            else 
            {
                vec_character[0].y = vec_character[0].y + 1;
                cout << "Player movement on the right" << endl;
                place[vec_character[0].x][vec_character[0].y] = 'P';
                place_fulled[vec_character[0].x][vec_character[0].y] = true;
                place_fulled[temp_x][temp_y] = false;
                place[temp_x][temp_y] = '.';
                hit_streak_damage = 0;
            }
        }
    }
    else if (action == 'U') // Движение вверх
    {
        if (vec_character[0].x - 1 < 0) { return; }
        else 
        {
            temp_x = vec_character[0].x;
            temp_y = vec_character[0].y;
            if (place_fulled[vec_character[0].x -1][vec_character[0].y] == true) 
            {
                player_position_x = vec_character[0].x -1;
                player_position_y = vec_character[0].y;
                cout << "Find enemy!" << endl;
                int itr = find_iterator_enemy(vec_character, player_position_x, player_position_y);
                damage_person(vec_character, itr, place_fulled, place, score, kills, star, hit_streak_damage);
                hit_streak_damage += 15;
            } 
            else 
            {
                vec_character[0].x = vec_character[0].x - 1;
                cout << "Player movement on the up" << endl;
                place[vec_character[0].x][vec_character[0].y] = 'P';
                place_fulled[vec_character[0].x][vec_character[0].y] = true;
                place_fulled[temp_x][temp_y] = false;
                place[temp_x][temp_y] = '.';
                hit_streak_damage = 0;
            }
        }
    }
    else if (action == 'D') // Движение вниз
    {
        if (vec_character[0].x + 1 >= 19) { return; }
        else 
        {
            temp_x = vec_character[0].x;
            temp_y = vec_character[0].y;
            if (place_fulled[vec_character[0].x + 1][vec_character[0].y] == true) 
            {
                player_position_x = vec_character[0].x + 1;
                player_position_y = vec_character[0].y;
                cout << "Find enemy!" << endl;
                int itr = find_iterator_enemy(vec_character, player_position_x, player_position_y);
                damage_person(vec_character, itr, place_fulled, place, score, kills, star, hit_streak_damage);
                hit_streak_damage += 15;
            } 
            else 
            {
                vec_character[0].x = vec_character[0].x + 1;
                cout << "Player movement on the down" << endl;  
                place[vec_character[0].x][vec_character[0].y] = 'P';
                place_fulled[vec_character[0].x][vec_character[0].y] = true;
                place_fulled[temp_x][temp_y] = false;
                place[temp_x][temp_y] = '.';
                hit_streak_damage = 0;
            }
        }
    }
    else cout << "Incorrect action. Try again." << endl;
}

void damage_to_player (vector<character>& vec_character, bool (&place_fulled)[20][20], char (&place)[20][20], int &score, int &kills, int &iter, bool &player_life, player_star_power star)
{
    int count_last_chance = 0;
    int enemy_damage = vec_character[iter].damage;

    if (vec_character[0].armor > 0) 
    {
        vec_character[0].armor -= enemy_damage;
        
        if (vec_character[0].armor < 0) 
        {
            // Броня пробита, остаток урона идет на здоровье
            int leftover_damage = -vec_character[0].armor;
            cout << "Your armor break! " << leftover_damage << " damage goes to health" << endl;
            
            vec_character[0].armor = 0;
            vec_character[0].health -= leftover_damage;
        }
        else 
        {
            cout << enemy_damage << " damage to armor" << endl;
            cout << "Your armor left: " << vec_character[0].armor << endl;
            return; // Вы живы, выходим
        }
    }
    else 
    {
        // Нет брони - весь урон на здоровье
        cout << enemy_damage << " damage to health" << endl;
        vec_character[0].health -= enemy_damage;
    }

    if (vec_character[0].health <= 0) // обилка последний шанс
    {
        if (star.last_chanсe && count_last_chance <= 0) 
        {
            cout << "=*= LAST CHANCE ACTIVATED! =*= " << endl;
            count_last_chance = 1;
        }
    }

    if (vec_character[0].health <= 0) // Если умер
    {
        
        vec_character[0].health = 0;
        player_life = false;
    }
    else 
    {
        cout << "Your health left: " << vec_character[0].health << endl;
    }
}

void enemy_movement (char (&place)[20][20], bool (&place_fulled)[20][20], vector <character> &vec_character, int &score, int &kills, bool &player_life, player_star_power star) 
{
    int temp_x, temp_y, iter;

    for (int i = 1; i <= 9; i++) 
    {
        int rand_action = rand() % 4;
        if (rand_action == 0) // Движение влево
        {
            if (vec_character[i].y - 1 < 0);
            else 
            {
                if (vec_character[i].name == "DEAD") continue;
                temp_x = vec_character[i].x;
                temp_y = vec_character[i].y;

                if (place_fulled[vec_character[i].x][vec_character[i].y - 1] == true 
                      && (vec_character[i].x == vec_character[0].x && vec_character[i].y-1 == vec_character[0].y)) 
                {
                    cout << "Enemy seek you. Attack you!" << endl;
                    iter = i;
                    damage_to_player(vec_character, place_fulled, place, score, kills, iter, player_life, star);
                }
                else if (place_fulled[vec_character[i].x][vec_character[i].y - 1] == false)
                {
                    vec_character[i].y = vec_character[i].y - 1;
                    place[vec_character[i].x][vec_character[i].y] = 'E';
                    place_fulled[vec_character[i].x][vec_character[i].y] = true;

                    place[temp_x][temp_y] = '.';
                    place_fulled[temp_x][temp_y] = false;
                }
            }   
        }
        else if (rand_action == 1) // Движение направо
        {
            if (vec_character[i].y + 1 >= 19 );
            else 
            {
                if (vec_character[i].name == "DEAD") continue;
                temp_x = vec_character[i].x;
                temp_y = vec_character[i].y;

                if (place_fulled[vec_character[i].x][vec_character[i].y + 1] == true 
                      && (vec_character[i].x == vec_character[0].x && vec_character[i].y+1 == vec_character[0].y)) 
                {
                    cout << "Enemy seek you. Attack you!" << endl;
                    iter = i;
                    damage_to_player(vec_character, place_fulled, place, score, kills, iter, player_life, star);
                }
                else if (place_fulled[vec_character[i].x][vec_character[i].y + 1] == false)
                {
                    vec_character[i].y = vec_character[i].y + 1;
                    place[vec_character[i].x][vec_character[i].y] = 'E';
                    place_fulled[vec_character[i].x][vec_character[i].y] = true;

                    place[temp_x][temp_y] = '.';
                    place_fulled[temp_x][temp_y] = false;
                }
            }
        }
        else if (rand_action == 2) // Движение вверх
        {
            if (vec_character[i].x - 1 < 0);
            else 
            {
                if (vec_character[i].name == "DEAD") continue;
                temp_x = vec_character[i].x;
                temp_y = vec_character[i].y;

                if (place_fulled[vec_character[i].x - 1][vec_character[i].y] == true 
                      && (vec_character[i].x == vec_character[0].x -1 && vec_character[i].y == vec_character[0].y)) 
                {
                    cout << "Enemy seek you. Attack you!" << endl;
                    iter = i;
                    damage_to_player(vec_character, place_fulled, place, score, kills, iter, player_life, star);
                }
                else if (place_fulled[vec_character[i].x - 1][vec_character[i].y] == false)
                {
                    vec_character[i].x = vec_character[i].x - 1;
                    place[vec_character[i].x][vec_character[i].y] = 'E';
                    place_fulled[vec_character[i].x][vec_character[i].y] = true;

                    place[temp_x][temp_y] = '.';
                    place_fulled[temp_x][temp_y] = false;
                }
            }
        }
        else if (rand_action == 3) // Движение вниз
        {
            if (vec_character[i].x + 1 >= 19);
            else 
            {
                if (vec_character[i].name == "DEAD") continue;
                temp_x = vec_character[i].x;
                temp_y = vec_character[i].y;

                if (place_fulled[vec_character[i].x + 1][vec_character[i].y] == true 
                      && (vec_character[i].x == vec_character[0].x +1 && vec_character[i].y == vec_character[0].y)) 
                {
                    cout << "Enemy seek you. Attack you!" << endl;
                    iter = i;
                    damage_to_player(vec_character, place_fulled, place, score, kills, iter, player_life, star);
                }
                else if (place_fulled[vec_character[i].x + 1][vec_character[i].y] == false)
                {
                    vec_character[i].x = vec_character[i].x + 1;
                    place[vec_character[i].x][vec_character[i].y] = 'E';
                    place_fulled[vec_character[i].x][vec_character[i].y] = true;

                    place[temp_x][temp_y] = '.';
                    place_fulled[temp_x][temp_y] = false;
                }
            }
        }
        else // исключение ошибок
        {
            cout << "Error code 100: error rand system." << endl;
        }
    }
    cout << "Enemys actions movement!" << endl;
}

void sync_map_with_characters(char (&place)[20][20], bool (&place_fulled)[20][20], const vector<character>& characters) 
{
    // Очищаем всю карту
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            place[i][j] = '.';
            place_fulled[i][j] = false;
        }
    }
    
    // Ставим игрока
    place[characters[0].x][characters[0].y] = 'P';
    place_fulled[characters[0].x][characters[0].y] = true;
    
    // Ставим всех живых врагов
    for (size_t i = 1; i < characters.size(); i++) {
        if (characters[i].name != "DEAD") {
            place[characters[i].x][characters[i].y] = 'E';
            place_fulled[characters[i].x][characters[i].y] = true;
        }
    }
}

bool checking_how_many_enemies (vector <character> &vec_character) 
{
    for (int i = 1; i <= 9; i++) 
    {
        if (vec_character[i].name != "DEAD") return false;
    }
    return true;
}

void check_star_power (char &action, player_star_power star) 
{
    if (star.furious_hero || star.last_chanсe || star.living_steel || star.soul_strength) 
    {
        if (star.soul_strength) 
        {
            cout << "       STAR POWER  № 1     " << endl;
            cout << "===========================" << endl;
            cout << "||     SOUL STRENGHT     ||" << endl;
            cout << "|| - - - - - - - - - - - ||" << endl;
            cout << "||  For every enemy you  ||" << endl;
            cout << "||  kill, you get 30     ||" << endl;
            cout << "||  health points        ||" << endl;
            cout << "===========================" << endl;
            cout << endl;
        }
        if (star.furious_hero) 
        {
            cout << "       STAR POWER  № 2     " << endl;
            cout << "===========================" << endl;
            cout << "||      FURIOUS HERO     ||" << endl;
            cout << "|| - - - - - - - - - - - ||" << endl;
            cout << "|| Damage increases by   ||" << endl;
            cout << "|| 15, for each next hit ||" << endl;
            cout << "|| in the series         ||" << endl;
            cout << "===========================" << endl;
            cout << endl;
        }
        if (star.last_chanсe) 
        {
            cout << "       STAR POWER  № 3     " << endl;
            cout << "===========================" << endl;
            cout << "||      LAST CHANCE      ||" << endl;
            cout << "|| - - - - - - - - - - - ||" << endl;
            cout << "|| If you take damage,   ||" << endl;
            cout << "|| that would kill you,  ||" << endl;
            cout << "|| your health will be   ||" << endl;
            cout << "|| reset to 1 point,     ||" << endl;
            cout << "|| only once             ||" << endl;
            cout << "===========================" << endl;
            cout << endl;
        }
        if (star.last_chanсe) 
        {
            cout << "       STAR POWER  № 4     " << endl;
            cout << "===========================" << endl;
            cout << "||      LIVING STEEL     ||" << endl;
            cout << "|| - - - - - - - - - - - ||" << endl;
            cout << "|| Your armor is restored||" << endl;
            cout << "|| by 5 points for each  ||" << endl;
            cout << "|| hit on an enemy       ||" << endl;
            cout << "===========================" << endl;
            cout << endl;
        }
    } 
    else 
    {
        cout << "You don't have any unlocked star powers." << endl;
    }
}

void save_game (vector <character> &vec_character, player_star_power &star, int &count_games, int &score, int &kills) 
{
    ofstream file ("save.bin", ios::binary);

    file.write((char*)&count_games, sizeof(int)); // всего игр (для абилок)

    // сохраняем сами абилки

    file.write((char*)&star.furious_hero, sizeof(bool));
    file.write((char*)&star.last_chanсe, sizeof(bool));
    file.write((char*)&star.living_steel, sizeof(bool));
    file.write((char*)&star.soul_strength, sizeof(bool));

    // сохраняем перснажей

    int count_characters = vec_character.size();
    file.write((char*)&count_characters, sizeof(int));

    for (int i = 0; i <= 9; i++) 
    {
        file.write((char*)&vec_character[i].health, sizeof(int));
        file.write((char*)&vec_character[i].armor, sizeof(int));
        file.write((char*)&vec_character[i].damage, sizeof(int));
        file.write((char*)&vec_character[i].x, sizeof(int));
        file.write((char*)&vec_character[i].y, sizeof(int));
        if (i == 0) 
        {
            file.write((char*)&score, sizeof(int));
            file.write((char*)&kills, sizeof(int));
        }

        int len_str = vec_character[i].name.length();
        file.write((char*)&len_str, sizeof(int));
        file.write(vec_character[i].name.c_str(), len_str);
    }

    file.close();
    cout << "-- Game saved! --" << endl;
}

void load_game (vector <character> &vec_character, player_star_power &star, int &count_games, int &score, int &kills) 
{
    ifstream file ("save.bin", ios::binary);

    if (!file) 
    {
        cout << "Save file is not found!" << endl;
        return;
    }

    file.read((char*)&count_games, sizeof(int)); // всего игр (для абилок)

    // читаем абилки

    file.read((char*)&star.furious_hero, sizeof(bool));
    file.read((char*)&star.last_chanсe, sizeof(bool));
    file.read((char*)&star.living_steel, sizeof(bool));
    file.read((char*)&star.soul_strength, sizeof(bool));

    // читаем персонажей

    int count_character;
    file.read((char*)&count_character, sizeof(int));
    vec_character.resize(count_character);

    for (int i = 0; i <= 9; i++) 
    {
        file.read((char*)&vec_character[i].health, sizeof(int));
        file.read((char*)&vec_character[i].armor, sizeof(int));
        file.read((char*)&vec_character[i].damage, sizeof(int));
        file.read((char*)&vec_character[i].x, sizeof(int));
        file.read((char*)&vec_character[i].y, sizeof(int));
        if (i == 0) 
        {
            file.read((char*)&score, sizeof(int));
            file.read((char*)&kills, sizeof(int));
        }
        int len_str;
        file.read((char*)&len_str, sizeof(int));
        vector<char> name_buf(len_str + 1);
        file.read(name_buf.data(), len_str);
        name_buf[len_str] = '\0';
        vec_character[i].name = name_buf.data();
    }
    file.close();
    cout << "-- Game load! --" << endl;
}

int main () 
{
    player_star_power star;
    vector <character> vec_character (10);
    character person;
    int count_games = 0;
    int score, kills;
    int temp_health, temp_armor, temp_damage;
    bool game_loaded = false;
    bool if_game_loaded = false;

    // Проверка на файл сохранения

    cout << "Check save file.." << endl;
    ifstream file("save.bin", ios::binary);
    if (file.good()) 
    {
        file.close();
        char choice;
        cout << "Save found! Load? (y/n): ";
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            load_game(vec_character, star, count_games, score, kills);
            game_loaded = true;
            
            // Берем данные игрока из загрузки
            temp_health = vec_character[0].health;
            temp_armor = vec_character[0].armor;
            temp_damage = vec_character[0].damage;
            
            cout << "Game loaded! Player: " << vec_character[0].name << endl;
        }
    }

    //Создаем главного героя
    if (!game_loaded) 
    {
        cout << "== Create your character == " << endl;
        cout << "Write name character -> ";
        cin >> vec_character[0].name;
        
        while (true) // Здоровье
        {
            cout << "Input health character (50-150) -> ";
            cin >> temp_health;
            if (temp_health >= 50 && temp_health <= 150) 
            {
                vec_character[0].health = temp_health;
                break;
            }
            else cout << "Incorrect value health. Try again." << endl;
        }

        while (true) // Броня
        {
            cout << "Input armor for character (0-50) -> ";
            cin >> temp_armor;
            if (temp_armor >= 0 && temp_armor <= 50) 
            {
                vec_character[0].armor = temp_armor;
                break;
            }
            else cout << "Incorrect value armor. Try again." << endl;
        }

        while (true) // Урон
        {
            cout << "Input damage for character (15-30) -> ";
            cin >> temp_damage;
            if (temp_damage >= 15 && temp_damage <= 30) 
            {
                vec_character[0].damage = temp_damage;
                break;
            }
            else cout << "Incorrect value damage. Try again." << endl;
        }
        cout << "--Great! Character has been created." << endl;
    }

    while (true) // игровой цикл
    {
        bool win_games = false;
        bool player_life = true;
        char play_place [20][20];
        bool play_place_fulled [20][20] = {false};

        if (game_loaded) 
        {
            // загружаем карту
            sync_map_with_characters(play_place, play_place_fulled, vec_character);
            input_play_place(play_place, score, kills, vec_character);
            if_game_loaded = true;
        }
        else 
        {
            // Выдаем предыдущие характеристики игроку  
            kills = 0;
            score = 0;
            vec_character[0].armor = temp_armor;
            vec_character[0].damage = temp_damage;
            vec_character[0].health = temp_health;

            if (if_game_loaded) 
            {
                vec_character[0].armor = 40;
                vec_character[0].damage = 20;
                vec_character[0].health = 120;
            }

            srand(time(nullptr));

            for (int i = 0; i < 19; i++) 
            {
                for (int j = 0; j < 19; j++) 
                {
                    play_place[i][j] = '.';
                }
            }

            cout << "Randoming coordinate..." << endl;
            vec_character[0].x = rand() % 19;
            vec_character[0].y = rand() % 19;
            play_place[vec_character[0].x][vec_character[0].y] = 'P';
            play_place_fulled[vec_character[0].x][vec_character[0].y] = true;

            input_play_place(play_place, score, kills, vec_character);

            // Рандомим врагов
            for (int i = 1; i <= 9; i++) 
            {
                // характеристики

                vec_character[i].name = "Enemy #" + to_string(i);
                vec_character[i].health = rand() % 101 + 50;
                vec_character[i].armor = rand() % 51;
                vec_character[i].damage = rand() % 16 + 15;

                // позиция
                while (true) 
                {
                    vec_character[i].x = rand() % 19;
                    vec_character[i].y = rand() % 19;
                    if (play_place_fulled[vec_character[i].x][vec_character[i].y] == false) break;
                }
                play_place[vec_character[i].x][vec_character[i].y] = 'E';
                play_place_fulled[vec_character[i].x][vec_character[i].y] = true;
            }
            input_play_place(play_place, score, kills, vec_character);
        }

        // Игровая механика
        cout << " --- START GAME ---" << endl;
        bool end_save_game = false;
        while (true) 
        {
            char action;
            while (true) 
            {
                cout << "Input action (L - R - U - D / C - check star power / S - save game) -> ";
                cin >> action;
                if (action == 'L' || action == 'R' || action == 'U' || action == 'D' ) break;
                else if (action == 'C') 
                {
                    check_star_power(action, star);
                }
                else if (action == 'S') 
                {
                    cout << "Try save game.." << endl;
                    save_game(vec_character, star, count_games, score, kills);
                    end_save_game = true;
                    break;
                }
                else cout << "Incorrect action. Try again." << endl;
            }
            if (end_save_game) break;
            cout << "- - - - - - - - - - - - - - - - - - - - - " << endl;
            player_movement(play_place, play_place_fulled, vec_character, action, score, kills, star);
            sync_map_with_characters(play_place, play_place_fulled, vec_character);
            if (checking_how_many_enemies(vec_character) == true) 
            {
                cout << "-----------------------" << endl;
                cout << "-  Congruatulations!  -" << endl;
                cout << "-       You win!      -" << endl;
                cout << "-----------------------" << endl;
                cout << "==== Weel played! ==== " << endl;
                cout << "-score: " << score << endl;
                cout << "-kills: " << kills << endl;
                cout << "=======================" << endl;
                win_games = true;
                break;
            }
            else 
            {
                enemy_movement(play_place, play_place_fulled, vec_character, score, kills, player_life, star);
                sync_map_with_characters(play_place, play_place_fulled, vec_character);
                input_play_place(play_place, score, kills, vec_character);
                if (player_life == false) 
                {
                    cout << "-----------------------" << endl;
                    cout << "- You die! Game Over. -" << endl;
                    cout << "-----------------------" << endl;
                    cout << "==== Weel played! ==== " << endl;
                    cout << "-score: " << score << endl;
                    cout << "-kills: " << kills << endl;
                    cout << "=======================" << endl;
                    break;
                }
                
            }
        }
        if (end_save_game) break;
        else
        {
            game_loaded = false;
            cout << "You want continue? (0 - no / 1 - yes): ";
            int action_continue;
            cin >> action_continue;
            if (action_continue == 0) break;
        
            // Получение звездных сил

            if ((count_games >= 3 || win_games) && star.soul_strength == false) 
            {
                cout << endl;
                cout << "   UNLOCK STAR POWER  № 1  " << endl;
                cout << "===========================" << endl;
                cout << "||     SOUL STRENGHT     ||" << endl;
                cout << "|| - - - - - - - - - - - ||" << endl;
                cout << "||  For every enemy you  ||" << endl;
                cout << "||  kill, you get 30     ||" << endl;
                cout << "||  health points        ||" << endl;
                cout << "===========================" << endl;
                cout << endl;
                star.soul_strength = true;
            }
            if ((count_games >= 6 || win_games) && star.furious_hero == false) 
            {
                cout << endl;
                cout << "   UNLOCK STAR POWER  № 2  " << endl;
                cout << "===========================" << endl;
                cout << "||      FURIOUS HERO     ||" << endl;
                cout << "|| - - - - - - - - - - - ||" << endl;
                cout << "|| Damage increases by   ||" << endl;
                cout << "|| 15, for each next hit ||" << endl;
                cout << "|| in the series         ||" << endl;
                cout << "===========================" << endl;
                cout << endl;
                star.furious_hero = true;
            }
            if ((count_games >= 9 || win_games) && star.last_chanсe == false) 
            {
                cout << endl;
                cout << "   UNLOCK STAR POWER  № 3  " << endl;
                cout << "===========================" << endl;
                cout << "||      LAST CHANCE      ||" << endl;
                cout << "|| - - - - - - - - - - - ||" << endl;
                cout << "|| If you take damage,   ||" << endl;
                cout << "|| that would kill you,  ||" << endl;
                cout << "|| your health will be   ||" << endl;
                cout << "|| reset to 1 point,     ||" << endl;
                cout << "|| only once             ||" << endl;
                cout << "===========================" << endl;
                cout << endl;
                star.last_chanсe = true;
            }
            if ((count_games >= 12 || win_games) && star.living_steel == false) 
            {
                cout << endl;
                cout << "  UNLOCK STAR POWER  № 4   " << endl;
                cout << "===========================" << endl;
                cout << "||      LIVING STEEL     ||" << endl;
                cout << "|| - - - - - - - - - - - ||" << endl;
                cout << "|| Your armor is restored||" << endl;
                cout << "|| by 5 points for each  ||" << endl;
                cout << "|| hit on an enemy       ||" << endl;
                cout << "===========================" << endl;
                cout << endl;
                star.living_steel = true;
            }

            if (star.furious_hero && star.last_chanсe && star.living_steel && star.soul_strength && win_games) 
            {
                cout << "You bravely kill the last enemy, ";
                cout << "and your four star powers sparkle and form a stream of light. " << endl;
                cout << "This power shatters the time loop you've been trapped in for " << count_games << "games." << endl;
                cout << "Thanks for playing!" << endl;
                break;
            }
            win_games = false;
            count_games++;
        }
    }
    return 0;
}