#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
struct WeaponCounter
{
    WeaponCounter() = default;
    void Print1()
    {
        std::cout << "Weapon Usage status" << std::endl;
        Print2();
    }
    void Print2()
    {
        std::cout << "\tDagger      / " << dagger << std::endl;
        std::cout << "\tDefault Gun / " << default_gun << std::endl;
        std::cout << "\tShotGun     / " << shotgun << std::endl;
    }
    void PrintSum()
    {
        std::cout << "\tTotal       / " << GetSum() << std::endl;
        std::cout << std::endl;
    }
    unsigned GetSum() const { return dagger + default_gun + shotgun; }
    unsigned dagger = 0, default_gun = 0, shotgun = 0;
};

struct ItemCounter
{
    ItemCounter() = default;
    void Print()
    {
        std::cout << "Item Usage status" << std::endl;
        std::cout << "\tBomb   / " << bomb << std::endl;
        std::cout << "\tPotion / " << potion << std::endl;
    }
    unsigned bomb = 0, potion = 0;
};

struct DamageCounter
{
    DamageCounter() = default;
    void Print()
    {
        std::cout << "Damaged status" << std::endl;
        std::cout << "\tDamage Tile   / " << damage_tile << std::endl;
        std::cout << "\tWarrior       / " << warrior << std::endl;
        std::cout << "\tScout         / " << scout << std::endl;
        std::cout << "\tTanker        / " << tanker << std::endl;
        std::cout << "\tBoss Magicion / " << boss_magicion << std::endl;
    }
    unsigned warrior = 0, tanker = 0, scout = 0, boss_magicion = 0, damage_tile = 0;
};

struct KillCounter
{
    KillCounter() = default;
    void Print()
    {
        std::cout << "Kill Counter" << std::endl;
        std::cout << "\tWarrior" << std::endl;
        warrior.Print2();
        warrior.PrintSum();
        std::cout << "\tScout" << std::endl;
        scout.Print2();
        scout.PrintSum();
        std::cout << "\tTanker" << std::endl;
        tanker.Print2();
        tanker.PrintSum();
        std::cout << "\tBoss Magicion" << std::endl;
        boss_magicion.Print2();
        boss_magicion.PrintSum();
    }
    WeaponCounter warrior, tanker, scout, boss_magicion;
};

struct LevelInfo
{
    ItemCounter item_usage;
    WeaponCounter weapon_usage;
    DamageCounter damaged;
    KillCounter killed;

    float play_time = 0.f;

    unsigned player_start_health = 0;

    std::string Level_data;

    std::string dead_by;

    void Print()
    {
        std::cout << "Using Map : " << Level_data << std::endl;
        std::cout << "Play Time : " << play_time << std::endl;
        std::cout << "Start HP  : " << player_start_health << std::endl;

        item_usage.Print();
        std::cout << std::endl;
        weapon_usage.Print1();
        std::cout << std::endl;
        damaged.Print();
        std::cout << std::endl;
        killed.Print();
        std::cout << std::endl;

        if(!dead_by.empty())
        {
            std::cout << "Player dead by " << dead_by << std::endl;
        }
    }
};

struct Try
{
    void Print()
    {
        for(int i = 0; i < levels.size(); ++i)
        {
            std::cout << "--------------------------Lvl " + std::to_string(i + 1) + "--------------------------\n";
            levels[i].Print();
            std::cout << "---------------------------------------------------------\n";
        }
    };
    std::vector<LevelInfo> levels;
};


class Analyzer
{
public:
    void Analyze(std::string&& file_path);

    void Print()
    {
        for(int i = 0; i < tries.size(); ++i)
        {

                std::cout << "=========================================================\n";
                std::cout << "==========================Try " + std::to_string(i + 1) + "==========================\n";
                tries[i].Print();
                std::cout << "=========================================================\n";

        }
        std::cout << "===================Analyzing Ended=======================\n";
    }
private:
    std::vector<Try> tries;
};

void Analyzer::Analyze(std::string&& file_path)
{


    std::ifstream ifs;
    ifs.open(std::forward<std::string>(file_path));

    Try* cur_try = nullptr;

    if (ifs.is_open())
    {
        while(!ifs.eof())
        {
            std::string line;

            static auto HasData = [&line](const std::string& data)->bool
            {
                return line.find(data) < line.length();
            };

            static auto HasData2 = [](const std::string& line, const std::string& data)->bool
            {
                return line.find(data) < line.length();
            };


            std::getline(ifs, line);
            line = line.substr(line.find('\t') + 1);
            if(line == "MainMenuState Loaded")
            {
                tries.emplace_back(Try());
                cur_try = &tries.back();
                continue;
            }

            if(HasData("Using map"))
            {
                std::string levelDataPath = line.substr(line.find_first_of('/') + 2);
                LevelInfo levelInfo;

                levelInfo.Level_data = levelDataPath;

                line.clear();

                float last_time = 0.f;
                std::string lastlyDamaged;
                std::string lastlyUsedWeapon;
                while(!ifs.eof())
                {

                    std::getline(ifs, line);
                    line = line.substr(line.find('\t') + 1);
                    if(line.empty())
                    {
                        continue;
                    }

                    if(HasData("UnLoaded") && !HasData("InGameMenuState"))
                    {
                        levelInfo.play_time += last_time;
                        cur_try->levels.emplace_back(levelInfo);
                        break;
                    }

                    static const std::string TimeStr{ "Time" };
                    if (HasData(TimeStr))
                    {

                        std::string timeStr = line.substr(line.find(TimeStr) + TimeStr.length());

                        if(HasData2(timeStr, "/"))
                        {
                            timeStr = timeStr.substr(timeStr.find('/') + 1);
                        }

                        last_time = std::stof(timeStr);

                        if (levelInfo.play_time == 0.f)
                        {
                            levelInfo.play_time = -last_time;
                            continue;
                        }
                    }

                    if(HasData("HP"))
                    {
                        line = line.substr(line.find('/') + 1);
                        int health = std::stoi(line);
                        levelInfo.player_start_health = health;
                        continue;
                    }

                    static const std::string weaponUsedString{ "Player used weapon " };
                    if(HasData(weaponUsedString))
                    {
                        line = line.substr(weaponUsedString.length());
                        std::stringstream l{ line };

                        std::string weaponName;
                        l >> weaponName;

                        //weapon Type
                        if (weaponName == "Dagger")
                        {
                            ++levelInfo.weapon_usage.dagger;
                        }
                        else if(weaponName == "BasicGun")
                        {
                            ++levelInfo.weapon_usage.default_gun;
                        }
                        else if(weaponName == "ShotGun")
                        {
                            ++levelInfo.weapon_usage.shotgun;
                        }
                        lastlyUsedWeapon = weaponName;
                        continue;
                    }

                    static const std::string itemUsedString{ "Player used item " };
                    if (HasData(itemUsedString))
                    {
                        line = line.substr(itemUsedString.length());
                        std::stringstream l{ line };

                        std::string itemType;
                        l >> itemType;

                        if (itemType == "Bomb")
                        {
                            ++levelInfo.item_usage.bomb;
                        }
                        else if (itemType == "Potion")
                        {
                            ++levelInfo.item_usage.potion;
                        }
                        continue;
                    }

                    static const std::string damagedString{ "Player Hit By" };
                    if(HasData(damagedString))
                    {
                        line = line.substr(damagedString.length());
                        std::stringstream l{ line };

                        std::string damageType;
                        l >> damageType;
                        lastlyDamaged = damageType;

                        if(damageType == "Warrior")
                        {
                            ++levelInfo.damaged.warrior;
                        }
                        else if (damageType == "Tanker")
                        {
                            ++levelInfo.damaged.tanker;
                        }
                        else if (damageType == "Scout's")
                        {
                            ++levelInfo.damaged.scout;
                        }
                        else if(damageType == "BossMagician")
                        {
                            ++levelInfo.damaged.boss_magicion;
                        }
                        else if(damageType == "DamageTile")
                        {
                            ++levelInfo.damaged.damage_tile;
                        }
                        continue;
                    }

                    static const std::string enemyString = "Enemy ";
                    static const std::string deadByString = "dead by ";
                    if(HasData(enemyString) && HasData(deadByString))
                    {
                        line = line.substr(line.find_first_of(enemyString) + enemyString.length());

                        std::stringstream l{ line };

                        std::string enemyType;

                        l >> enemyType;

                        WeaponCounter* pKilledEnemy = nullptr;

                        if (enemyType == "Warrior")
                        {
                            pKilledEnemy = &levelInfo.killed.warrior;
                        }
                        else if (enemyType == "Tanker")
                        {
                            pKilledEnemy = &levelInfo.killed.tanker;
                        }
                        else if (enemyType == "Scout")
                        {
                            pKilledEnemy = &levelInfo.killed.scout;
                        }
                        else if (enemyType == "BossMagician")
                        {
                            pKilledEnemy = &levelInfo.killed.boss_magicion;
                        }

                        line = line.substr(line.find(deadByString) + deadByString.length());
                        std::stringstream l2{ line };
                        std::string weaponType;

                        l2 >> weaponType;
                        if (weaponType == "Dagger")
                        {
                            ++pKilledEnemy->dagger;
                        }
                        else if (weaponType == "Bullet")
                        {
                            if(lastlyUsedWeapon == "BasicGun")
                            {
                                ++pKilledEnemy->default_gun;
                            }
                            else if(lastlyUsedWeapon =="ShotGun")
                            {
                                ++pKilledEnemy->shotgun;
                            }
                        }
                        //else if (weaponType == "ShotGun")
                        //{
                        //    ++pKilledEnemy->shotgun;
                        //}
                        continue;
                    }


                    static const std::string deadCheckString = "Dead Check";
                    if(HasData(deadCheckString) || HasData("DeadCheck"))
                    {
                        levelInfo.dead_by = lastlyDamaged;
                        levelInfo.play_time += last_time;
                        cur_try->levels.emplace_back(levelInfo);
                        break;
                    }
                }



                continue;
            }
        }
    }

    std::vector<size_t> erasing_que;

    for(auto itr = tries.rbegin(); itr != tries.rend(); ++itr)
    {
        if(itr->levels.empty())
        {
            size_t where = abs(std::distance(tries.rend(), itr)) - 1;
            erasing_que.emplace_back(where);
            //tries.erase(tries.begin() + (abs(std::distance(tries.rend(), itr)) - 1));
        }
    }

    for(auto where : erasing_que)
    {
        tries.erase(tries.begin() + where);
    }
    ifs.close();
}