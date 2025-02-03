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
    player->base_attack = 10;
    player->attack = player->base_attack;
    player->gold = 50;
    player->exp = 0;
    player->next_level_exp = 100;
    for (int i = 0; i < 5; i++) {
        player->skills[i] = 0;
    }
}

// 產生敵人
Enemy generate_enemy() {
    Enemy enemy;
    int type = rand() % 3;
    if (type == 0) {
        sprintf(enemy.name, "史萊姆");
        enemy.hp = 50;
        enemy.attack = 5;
        enemy.exp_reward = 20;
        enemy.gold_reward = 10;
    } else if (type == 1) {
        sprintf(enemy.name, "哥布林");
        enemy.hp = 80;
        enemy.attack = 8;
        enemy.exp_reward = 30;
        enemy.gold_reward = 15;
    } else if (type == 2) {
        sprintf(enemy.name, "巨人");
        enemy.hp = 150;
        enemy.attack = 9;
        enemy.exp_reward = 80;
        enemy.gold_reward = 40;
    } else {
        sprintf(enemy.name, "狼人");
        enemy.hp = 120;
        enemy.attack = 12;
        enemy.exp_reward = 50;
        enemy.gold_reward = 25;
    }
    return enemy;
}

// 顯示狀態
void show_status(Player *player) {
    printf("\n--- %s 的狀態 ---\n", player->name);
    printf("等級: %d  HP: %d/%d  攻擊力: %d  金錢: %d  經驗值: %d/%d\n",
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
        player->attack = player->base_attack;
        printf("\n*** 恭喜升級! 等級: %d  最大生命: %d  攻擊力: %d ***\n",
               player->level, player->max_hp, player->attack);
    }
}
// 戰鬥系統
void battle(Player *player) {
    Enemy enemy = generate_enemy();
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
                {"火球", 1.5, 0, 50},
                {"雷擊", 2.0, 0, 80},
                {"治癒", 0.0, 30, 70},
                {"冰凍", 1.2, 0, 60},
                {"爆裂擊", 2.5, 0, 100}
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
                    int damage = player->attack * skills[skill_choice - 1].attack_multiplier;
                    printf("你施放 %s，造成 %d 傷害!\n", skills[skill_choice - 1].name, damage);
                    enemy.hp -= damage;
                }
            } else {
                printf("無效選擇!\n");
                continue;
            }
        }

        if (enemy.hp > 0) {
            printf("%s 反擊，對你造成 %d 傷害!\n", enemy.name, enemy.attack);
            player->hp -= enemy.attack;
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
        player->hp = player->max_hp / 2;
    }
}

// 商店系統
void shop(Player *player) {
    Weapon weapons[] = {
        {"鐵劍", 5, 50},
        {"鋼劍", 10, 100},
        {"魔劍", 20, 200}
    };

    Skill skills[] = {
        {"火球", 1.5, 0, 50},
        {"雷擊", 2.0, 0, 80},
        {"治癒", 0.0, 30, 70},
        {"冰凍", 1.2, 0, 60},
        {"爆裂擊", 2.5, 0, 100}
    };

    while (1) {
        printf("\n--- 商店 ---\n");
        printf("1. 買武器\n2. 買技能\n3. 離開\n");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            printf("選擇武器:\n");
            for (int i = 0; i < 3; i++) {
                printf("%d. %s (+%d 攻擊) - %d 金幣\n", i + 1, weapons[i].name, weapons[i].attack_bonus, weapons[i].price);
            }
            printf("0. 取消\n");

            int w_choice;
            scanf("%d", &w_choice);
            if (w_choice >= 1 && w_choice <= 3) {
                if (player->gold >= weapons[w_choice - 1].price) {
                    player->attack = player->base_attack + weapons[w_choice - 1].attack_bonus;
                    player->gold -= weapons[w_choice - 1].price;
                    printf("你購買並裝備了 %s!\n", weapons[w_choice - 1].name);
                } else {
                    printf("金幣不足!\n");
                }
            }
        } else if (choice == 2) {
            printf("選擇技能:\n");
            for (int i = 0; i < 5; i++) {
                if (!player->skills[i]) {
                    printf("%d. %s (%.0f%% 傷害) - %d 金幣\n", i + 1, skills[i].name, skills[i].attack_multiplier * 100, skills[i].price);
                }
            }
            printf("0. 取消\n");

            int s_choice;
            scanf("%d", &s_choice);
            if (s_choice >= 1 && s_choice <= 5) {
                if (player->gold >= skills[s_choice - 1].price) {
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

// 主函數
int main() {
    srand(time(NULL));
    Player player;
    init_player(&player);
    while (1) {
        show_status(&player);
        printf("\n1. 戰鬥\n2. 商店\n3. 離開\n");
        int choice;
        scanf("%d", &choice);
        if (choice == 1) battle(&player);
        else if (choice == 2) shop(&player);
        else break;
    }
    return 0;
}

