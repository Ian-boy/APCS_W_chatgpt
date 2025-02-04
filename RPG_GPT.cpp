#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 玩家結構
typedef struct {
    char name[50];
    int level;
    int hp;
    int max_hp;
    int attack;
    int base_attack;
    int gold;
    int exp;
    int next_level_exp;
    int skills[5]; // 0:火球, 1:雷擊, 2:治癒, 3:冰凍, 4:爆裂擊
    int weapons[5];
} Player;

// 敵人結構
typedef struct {
    char name[50];
    int hp;
    int attack;
    int exp_reward;
    int gold_reward;
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
    for (int i = 0; i < 5; i++) {
        player->skills[i] = 0;
    }
}

// 產生敵人
Enemy generate_enemy(Player *player) {
    Enemy enemy;
    int type;
    
    if (player->level >= 3) {
        type = rand() % 6; // 允許惡龍出現
    } else if (player->level >= 2) {
        type = rand() % 5; // 不包含惡龍
    } else {
        type = rand() % 2; // 只有史萊姆和哥布林
    }

    switch (type) {
        case 0:
            sprintf(enemy.name, "史萊姆");
            enemy.hp = 50;
            enemy.attack = 5;
            enemy.exp_reward = 20;
            enemy.gold_reward = 10;
            break;
        case 1:
            sprintf(enemy.name, "哥布林");
            enemy.hp = 80;
            enemy.attack = 8;
            enemy.exp_reward = 30;
            enemy.gold_reward = 15;
            break;
        case 2:
            sprintf(enemy.name, "巨人");
            enemy.hp = 150;
            enemy.attack = 9;
            enemy.exp_reward = 80;
            enemy.gold_reward = 40;
            break;
        case 3:
            sprintf(enemy.name, "大富翁哥布林");
            enemy.hp = 90;
            enemy.attack = 9;
            enemy.exp_reward = 40;
            enemy.gold_reward = 70;
            break;
        case 4:
            sprintf(enemy.name, "狼人");
            enemy.hp = 120;
            enemy.attack = 12;
            enemy.exp_reward = 50;
            enemy.gold_reward = 25;
            break;
        case 5:
            sprintf(enemy.name, "傳說中的惡龍");
            enemy.hp = 230;
            enemy.attack = 15;
            enemy.exp_reward = 150;
            enemy.gold_reward = 150;
            break;
    }
    return enemy;
}


// 顯示狀態
void show_status(Player *player) {
    printf("\n--- %s 的狀態 ---\n", player->name);
    printf("等級: %d  HP❤️: %d/%d  攻擊力🥊: %d  金錢💰: %d  經驗值: %d/%d\n",
           player->level, player->hp, player->max_hp, player->attack, player->gold,
           player->exp, player->next_level_exp);
}

// 升級系統
void level_up(Player *player) {
    if (player->exp >= player->next_level_exp) {
        player->level++;
        player->exp = 0;
        player->next_level_exp *= 1.5;
        player->max_hp += 20;
        player->hp = player->max_hp;
        player->base_attack += 5;
        player->attack += player->base_attack;
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
        printf("你: HP %d/%d\n", player->hp, player->max_hp);
        printf("%s: HP %d\n", enemy.name, enemy.hp);

        printf("\n選擇行動:\n1. 普通攻擊\n2. 使用技能\n");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            printf("你對 %s 造成 %d 傷害!\n", enemy.name, player->attack);
            enemy.hp -= player->attack;
        } else if (choice == 2) {
            Skill skills[] = {
                {"火球🔥", 1.5, 0, 50},
                {"雷擊⚡",2.0, 0, 80},
                {"治癒🏥", 0.0, 30, 70},
                {"冰凍🧊", 1.2, 0, 60},
                {"爆裂擊🧨", 2.5, 0, 100} // 這裡數值保持不變，命中率控制在後面
            };

            printf("選擇技能:\n");
            for (int i = 0; i < 5; i++) {
                if (player->skills[i]) {
                    printf("%d. %s (%.0f%% 傷害)\n", i + 1, skills[i].name, skills[i].attack_multiplier * 100);
                }
            }
            printf("0. 取消\n");

            int skill_choice;
            scanf("%d", &skill_choice);

            if (skill_choice >= 1 && skill_choice <= 5 && player->skills[skill_choice - 1]) {
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
    } else {
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
        {"火球🔥", 1.5, 0, 50},
        {"雷擊⚡",2.0, 0, 80},
        {"治癒🏥", 0.0, 30, 70},
        {"冰凍🧊", 1.2, 0, 60},
        {"爆裂擊🧨", 2.5, 0, 100}
    };

    while (1) {
        printf("\n--- 商店 ---\n");
        printf("1. 買武器⚔️\n2. 買技能🔥\n3. 離開\n");
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
                    printf("%d. %s (%.0f%% 傷害) - %d 金幣\n", i + 1, skills[i].name, skills[i].attack_multiplier * 100, skills[i].price);
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
        printf("你探索了一會兒，但什麼都沒發現。\n");
    }
}
// 主函數
int main() {
    srand(time(NULL));
    Player player;
    init_player(&player);
    while (1) {
        level_up(&player);
        show_status(&player);
        printf("\n1. 探索地圖\n2. 商店\n");
        int choice;
        scanf("%d", &choice);
        if (choice == 1) explore(&player);
        else if (choice == 2) shop(&player);
    }
    return 0;
}
