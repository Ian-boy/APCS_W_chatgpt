#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 玩家結構
typedef struct {
    char name[50];
    int level;
    int hp;
    int max_hp;
    int mp;
    int max_mp;
    int attack;
    int base_attack;
    int gold;
    int exp;
    int next_level_exp;
    int skills[5]; // 0:火球, 1:雷擊, 2:治癒, 3:冰凍, 4:爆裂擊
    int weapons[6];
    int key;
    int base_key;
    int stage;
} Player;

// 敵人結構
typedef struct {
    char name[50];
    int hp;
    int attack;
    int exp_reward;
    int gold_reward;
    int key_reward;
} Enemy;

// 武器結構
typedef struct {
    char name[30];
    int attack_bonus;
    int price;
} Weapon;

// 技能結構
typedef struct {
    char name[30];
    float attack_multiplier;
    int heal;
    int mp;
    int price;
} Skill;

// 初始化玩家
void init_player(Player *player) {
    printf("請輸入你的角色名稱: ");
    scanf("%s", player->name);
    player->level = 1;
    player->hp = 100;
    player->max_hp = 100;
    player->base_attack = 0;
    player->attack = 10;
    player->gold = 50;
    player->exp = 0;
    player->next_level_exp = 100;
    player->key = 0;
    player->base_key = 0;
    player->stage = 1;
    player->mp = 10;
    player->max_mp = 10;
    for (int i = 0; i < 5; i++) {
        player->skills[i] = 0;
    }
}

// 產生敵人
Enemy generate_enemy(Player *player) {
    Enemy enemy;
    int type = -1; // 初始化 type，避免未定義的情況

    if (player->stage == 1) {
        if (player->level >= 4) {
            type = rand() % 6; // 允許惡龍出現
        } else if (player->level >= 2) {
            type = rand() % 5; // 不包含惡龍
        } else {
            type = rand() % 2; // 只有史萊姆和哥布林
        }
    } else if (player->stage == 2) {
        if (player->level >= 7) {
            type = rand() % 6; 
        } else if (player->level >= 5) {
            type = rand() % 4; 
        } else {
            type = rand() % 2; 
        }
    }

    switch (type) {
        case 0:
            sprintf(enemy.name, player->stage == 1 ? "普通史萊姆" : "螢光史萊姆");
            enemy.hp = player->stage == 1 ? 50 : 100;
            enemy.attack = player->stage == 1 ? 5 : 10;
            enemy.exp_reward = player->stage == 1 ? 20 : 40;
            enemy.gold_reward = player->stage == 1 ? 10 : 20;
            enemy.key_reward = 0;
            break;
        case 1:
            sprintf(enemy.name, player->stage == 1 ? "哥布林" : "夜行哥布林");
            enemy.hp = player->stage == 1 ? 80 : 160;
            enemy.attack = player->stage == 1 ? 8 : 16;
            enemy.exp_reward = player->stage == 1 ? 30 : 60;
            enemy.gold_reward = player->stage == 1 ? 15 : 30;
            enemy.key_reward = 0;
            break;
        case 2:
            sprintf(enemy.name, player->stage == 1 ? "大富翁哥布林" : "螢光蘑菇怪");
            enemy.hp = player->stage == 1 ? 90 : 300;
            enemy.attack = player->stage == 1 ? 9 : 32;
            enemy.exp_reward = player->stage == 1 ? 40 : 200;
            enemy.gold_reward = player->stage == 1 ? 80 : 80;
            enemy.key_reward = 0;
            break;
        case 3:
            sprintf(enemy.name, player->stage == 1 ? "巨人" : "森林甲蟲");
            enemy.hp = player->stage == 1 ? 150 : 500;
            enemy.attack = player->stage == 1 ? 9 : 10;
            enemy.exp_reward = player->stage == 1 ? 80 : 170;
            enemy.gold_reward = player->stage == 1 ? 40 : 70;
            enemy.key_reward = 0;
            break;
        case 4:
            sprintf(enemy.name, player->stage == 1 ? "狼人" : "蘑菇怪");
            enemy.hp = player->stage == 1 ? 120 : 200;
            enemy.attack = player->stage == 1 ? 12 : 20;
            enemy.exp_reward = player->stage == 1 ? 50 : 150;
            enemy.gold_reward = player->stage == 1 ? 25 : 80;
            enemy.key_reward = 0;
            break;
        case 5:
            sprintf(enemy.name, player->stage == 1 ? "森林惡龍(BOOS)🐲" : "螢光蘑菇王(BOOS)🍄");
            enemy.hp = player->stage == 1 ? 230 : 400;
            enemy.attack = player->stage == 1 ? 40 : 70;
            enemy.exp_reward = player->stage == 1 ? 200 : 350;
            enemy.gold_reward = player->stage == 1 ? 200 : 350;
            enemy.key_reward = player->stage == 1 ? 1 : 2;
            break;
        default:
            printf("敵人生成錯誤，重新生成...\n");
            return generate_enemy(player); // 遇到錯誤時重新生成敵人
    }

    return enemy;
}



// 顯示狀態
void show_status(Player *player) {
    printf("\n--- %s 的狀態 ---\n", player->name);
    printf("等級: %d  HP❤️: %d/%d  MP️🪄 : %d/%d  攻擊力🥊: %d  金錢💰: %d  經驗值: %d/%d 目前關卡:%d\n",
           player->level, player->hp, player->max_hp, player->mp, player->max_mp, player->attack, player->gold,
           player->exp, player->next_level_exp,player->stage);
}

// 升級系統
void level_up(Player *player) {
    if (player->exp >= player->next_level_exp) {
        player->level++;
        player->exp = 0;
        player->next_level_exp *= 1.5;
        player->max_hp += 10;
        player->hp = player->max_hp;
        player->base_attack += 3;
        player->attack += player->base_attack;
        player->max_mp +=1;
        player->mp = player->max_mp;
        printf("\n*** 恭喜升級! 等級: %d  最大生命: %d  攻擊力: %d ***\n",
               player->level, player->max_hp, player->attack);
    }
}
//戰鬥系統
void battle(Player *player) {
    Enemy enemy = generate_enemy(player);
    int frozen_turn = 0;

    printf("\n你遇到了 %s! (HP: %d, 攻擊力: %d)\n", enemy.name, enemy.hp, enemy.attack);

    while (enemy.hp > 0 && player->hp > 0) {
        printf("\n=== 戰鬥中 ===\n");
        printf("你: HP❤️ %d/%d MP🪄 %d/%d 攻擊力🥊 %d\n", player->hp, player->max_hp, player->mp, player->max_mp, player->attack);
        printf("%s: HP❤️ %d 攻擊力🥊 %d\n", enemy.name, enemy.hp, enemy.attack);

        printf("\n選擇行動:\n1. 普通攻擊\n2. 使用技能\n");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            printf("你對 %s 造成 %d 傷害!\n", enemy.name, player->attack);
            enemy.hp -= player->attack;
        } else if (choice == 2) {
            Skill skills[] = {
                {"火球🔥", 1.5, 0, 1},
                {"雷擊⚡",2.0, 0, 3},
                {"治癒🏥", 0.0, 30, 2},
                {"冰凍🧊", 1.2, 0, 3},
                {"爆裂擊🧨", 2.5, 0, 4}
            };

            printf("選擇技能:\n");
            for (int i = 0; i < 5; i++) {
                if (player->skills[i]) {
                    printf("%d. %s (%.0f%% 傷害) - 使用時消耗 %d mp\n", i + 1, skills[i].name, skills[i].attack_multiplier * 100, skills[i].mp);
                }
            }
            printf("0. 取消\n");

            int skill_choice;
            scanf("%d", &skill_choice);

            if (skill_choice >= 1 && skill_choice <= 5 && player->skills[skill_choice - 1]) {
                if (skills[skill_choice - 1].mp <= player->mp){
                    player->mp -= skills[skill_choice - 1].mp;
                    if (skills[skill_choice - 1].heal > 0) {
                        printf("你施放 %s，恢復 %d HP!\n", skills[skill_choice - 1].name, skills[skill_choice - 1].heal);
                        player->hp += skills[skill_choice - 1].heal;
                        if (player->hp > player->max_hp) player->hp = player->max_hp;
                    } else {
                        // **爆裂擊 (50% 命中)**
                        if (skill_choice == 5) {
                            if (rand() % 2 == 0) { // 50% 機率命中
                                int damage = player->attack * skills[skill_choice - 1].attack_multiplier;
                                printf("你施放 %s，造成 %d 傷害!\n", skills[skill_choice - 1].name, damage);
                                enemy.hp -= damage;
                            } else {
                                printf("你施放 %s，但攻擊失敗了!\n", skills[skill_choice - 1].name);
                            }
                        } 
                        // **冰凍 (50% 機率成功)**
                        else if (skill_choice == 4) {
                            int damage = player->attack * skills[skill_choice - 1].attack_multiplier;
                            printf("你施放 %s，造成 %d 傷害!\n", skills[skill_choice - 1].name, damage);
                            enemy.hp -= damage;
                            if (rand() % 2 == 0) { 
                                printf("冰凍成功🧊! %s 無法行動 1 回合🥶!\n", enemy.name);
                                frozen_turn = 1;
                            } else {
                                printf("冰凍失敗! %s 仍然可以攻擊!\n", enemy.name);
                            }
                        }
                        // **其他技能**
                        else {
                            int damage = player->attack * skills[skill_choice - 1].attack_multiplier;
                            printf("你施放 %s，造成 %d 傷害!\n", skills[skill_choice - 1].name, damage);
                            enemy.hp -= damage;
                        }
                    }
                } else {
                    printf("你的MP不夠");
                    continue;
                }    
            } else {
                printf("無效選擇!\n");
                continue;
            }
        }

        if (enemy.hp > 0 && !frozen_turn) {
            printf("%s 反擊，對你造成 %d 傷害!\n", enemy.name, enemy.attack);
            player->hp -= enemy.attack;
        } else if (frozen_turn) {
            printf("%s 被冰凍，無法攻擊!\n", enemy.name);
            frozen_turn = 0; // 冰凍效果只持續一回合
        }
    }

    if (player->hp > 0) {
        printf("\n你打敗了 %s!\n", enemy.name);
        player->exp += enemy.exp_reward;
        player->gold += enemy.gold_reward;
        if (enemy.key_reward > 0){
            player->key=enemy.key_reward;
            if (player->key<player->base_key){
            player->key=player->base_key;
            }
            else {
                player->base_key=enemy.key_reward;
                printf("%s掉落了通往第%d關的鑰匙\n你檢你了鑰匙\n你解鎖了第%d關",enemy.name,enemy.key_reward+1,enemy.key_reward+1);
            }
        }
    }else {
        printf("\n你被打敗了...\n");
        printf("金幣歸零，你復活了!\n");
        player->gold = 0;
        player->hp = player->max_hp * 0.83;
    }
}
// 商店系統
void shop(Player *player) {
    Weapon weapons[] = {
        {"木劍️", 1,20},
        {"銅劍", 2, 40},
        {"鐵劍", 3, 60},
        {"鋼劍", 5, 100},
        {"魔劍", 10, 200}
    };

    Skill skills[] = {
        {"火球🔥", 1.5, 0, 1, 50},
        {"雷擊⚡",2.0, 0, 3, 80},
        {"治癒🏥", 0.0, 30, 2, 70},
        {"冰凍🧊", 1.2, 0, 3, 60},
        {"爆裂擊🧨", 2.5, 0, 4, 100}
    };

    while (1) {
        printf("\n--- 商店 ---\n");
        printf("1. 買武器⚔️\n2. 買技能🔥\n3. 回復1MP - 10金幣\n4. 離開\n");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) { // 買武器⚔️
            printf("選擇武器:\n");
            int available = 0;  
            for (int i = 0; i < 5; i++) {
                if (player->weapons[i] == 0) {  
                    printf("%d. %s (+%d 攻擊) - %d 金幣\n", i + 1, weapons[i].name, weapons[i].attack_bonus, weapons[i].price);
                    available = 1;
                }
            }
            if (!available) {
                printf("你已經買完所有武器了！\n");
                continue;
            }
            printf("0. 取消\n");

            int w_choice;
            scanf("%d", &w_choice);

            // **新增檢查: 若武器已購買，阻止購買**
            if (w_choice >= 1 && w_choice <= 5) {
                if (player->weapons[w_choice - 1] == 1) {  // **防止購買已購買的武器**
                    printf("你已經擁有 %s，不能再買！\n", weapons[w_choice - 1].name);
                } else if (player->gold >= weapons[w_choice - 1].price) {
                    player->attack += weapons[w_choice - 1].attack_bonus;
                    player->gold -= weapons[w_choice - 1].price;
                    player->weapons[w_choice - 1] = 1;  
                    printf("你購買並裝備了 %s!\n", weapons[w_choice - 1].name);
                } else {
                    printf("金幣不足!\n");
                }
            }

        } else if (choice == 2) { // 買技能🔥
            printf("選擇技能:\n");
            int available = 0;
            for (int i = 0; i < 5; i++) {
                if (player->skills[i] == 0) {
                    printf("%d. %s (%.0f%% 傷害) - %d  - 使用時消耗 %d mp\n", i + 1, skills[i].name, skills[i].attack_multiplier * 100, skills[i].price, skills[i].mp);
                    available = 1;
                }
            }
            if (!available) {
                printf("你已經學會所有技能了！\n");
                continue;
            }
            printf("0. 取消\n");

            int s_choice;
            scanf("%d", &s_choice);
            if (s_choice >= 1 && s_choice <= 5) {
                if (player->skills[s_choice - 1] == 1) {  // **防止購買已學會的技能**
                    printf("你已經學會 %s，不能再買！\n", skills[s_choice - 1].name);
                } else if (player->gold >= skills[s_choice - 1].price) {
                    player->skills[s_choice - 1] = 1;
                    player->gold -= skills[s_choice - 1].price;
                    printf("你學會了 %s!\n", skills[s_choice - 1].name);
                } else {
                    printf("金幣不足!\n");
                }
            }
        } else if (choice == 3) {
            if (player->gold >= 10){
                if (player->mp < player->max_mp){    
                    player->mp += 1;
                    player->gold -=10;
                    printf("你回復了1mp!");
                }else printf("你的mp已滿，不能回復");
            }else {
                printf("金幣不足!\n");
            }
        } else {
            break;
        }
    }
}


// 地圖探索系統
void explore(Player *player) {
    printf("\n=== 你開始探索地圖... ===\n");

    int event = rand() % 4; // 0: 遇敵, 1: 發現寶箱, 2: 無事發生

    if (event == 0) {
        printf("你遇到了一個敵人!\n");
        battle(player);
    } else if (event == 1) {
        int gold_found = (rand() % 50) + 10; // 獲得 10~50 金幣
        printf("你發現了一個寶箱! 內有 %d 金幣!\n", gold_found);
        player->gold += gold_found;
    } else if (event == 2) {
        int hp_found = (rand() % 40) + 10; // 獲得 10~50 金幣
        printf("你發現了一個無人的營帳! \n你在裡面休息了一晚上，回復了 %d HP❤️!\n", hp_found);
        player->hp += hp_found;
        if (player->hp > player->max_hp) player->hp = player->max_hp;
    } else {
        if (player->stage == 1){
            printf("你探索了一會兒，但什麼都沒發現。\n");
        }   
        else{
            int choice;
            printf("你發現了一個螢光蘑菇。\n是否吃掉個螢光蘑菇(有可能+hp or -hp or +exp)\n1.是\n2.否\n");
            scanf("%d",&choice);
            if (choice == 1){
                int mushroom = rand() % 3;
                if (mushroom == 0){
                    player->hp -= player->hp*0.6 ;
                    printf("你吃的蘑菇有毒,你減少了60趴的血量");
                }else if (mushroom == 1){
                    player->hp += player->hp*0.3 ;
                    if (player->hp > player->max_hp) player->hp = player->max_hp;
                    printf("你吃的蘑菇有回復效果,你回復了30趴的血量");
                }else{
                    player->exp += 50;
                    printf("你吃的蘑菇有經驗值,你增加了50經驗值");
                }    
            }
        }
    }
}
//關卡
void stage(Player *player){
    for (int i = 1; i < 6; i++){
        if (player->key+1>=i){
            printf("%d.第%d關\n",i,i);
        }
        else printf("%d.第%d關(未解鎖)\n",i,i);
    }
    int choice;
    scanf("%d", &choice);
    if (player->key+1>=choice){
        for (int i=1; i < 6; i++){
        if (choice == i){
        player->stage = player->key+1;}
        }
    }else printf("你還未解鎖這關");
}
// 主函數
int main() {
    srand(time(NULL));
    Player player;
    init_player(&player);
    while (1) {
        level_up(&player);
        show_status(&player);
        printf("\n1. 探索地圖\n2. 商店\n3.切換關卡\n");
        int choice;
        scanf("%d", &choice);
        if (choice == 1) explore(&player);
        else if (choice == 2) shop(&player);
        else if (choice == 3) stage(&player);
        else printf("無效選擇!\n");
    }

    return 0;
}
