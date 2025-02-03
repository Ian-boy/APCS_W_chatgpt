#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 定義玩家結構
typedef struct {
    char name[50];
    int level;
    int hp;
    int max_hp;
    int attack;
    int gold;
    int exp;
    int next_level_exp;
    int skills[3]; // 0:火球, 1:雷擊, 2:治癒 (是否擁有該技能)
} Player;

// 定義敵人結構
typedef struct {
    char name[50];
    int hp;
    int attack;
    int exp_reward;
    int gold_reward;
} Enemy;

// 定義武器
typedef struct {
    char name[30];
    int attack_bonus;
    int price;
} Weapon;

// 定義技能
typedef struct {
    char name[30];
    int damage;
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
    player->attack = 10;
    player->gold = 50;
    player->exp = 0;
    player->next_level_exp = 100;
    for (int i = 0; i < 3; i++) {
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
    } else {
        sprintf(enemy.name, "狼人");
        enemy.hp = 120;
        enemy.attack = 12;
        enemy.exp_reward = 50;
        enemy.gold_reward = 25;
    }
    return enemy;
}

// 顯示玩家狀態
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
        player->attack += 5;
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
            printf("選擇技能:\n");
            if (player->skills[0]) printf("1. 火球 (25 傷害)\n");
            if (player->skills[1]) printf("2. 雷擊 (40 傷害)\n");
            if (player->skills[2]) printf("3. 治癒 (+30HP)\n");
            printf("0. 取消\n");

            int skill_choice;
            scanf("%d", &skill_choice);

            if (skill_choice == 1 && player->skills[0]) {
                printf("你施放火球，造成 25 傷害!\n");
                enemy.hp -= 25;
            } else if (skill_choice == 2 && player->skills[1]) {
                printf("你施放雷擊，造成 40 傷害!\n");
                enemy.hp -= 40;
            } else if (skill_choice == 3 && player->skills[2]) {
                printf("你施放治癒，恢復 30 HP!\n");
                player->hp += 30;
                if (player->hp > player->max_hp) player->hp = player->max_hp;
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
        printf("獲得 %d 經驗值 和 %d 金幣!\n", enemy.exp_reward, enemy.gold_reward);
        level_up(player);
    } else {
        printf("\n你被打敗了...\n");
    }
}

// 商店系統
void shop(Player *player) {
    Skill skills[] = {
        {"火球", 25, 0, 50},
        {"雷擊", 40, 0, 80},
        {"治癒", 0, 30, 70}
    };

    printf("\n--- 商店 ---\n");
    printf("選擇要購買的技能:\n");
    printf("1. 火球 (25 傷害) - 50 金幣\n");
    printf("2. 雷擊 (40 傷害) - 80 金幣\n");
    printf("3. 治癒 (+30HP) - 70 金幣\n");
    printf("0. 離開\n");

    int choice;
    scanf("%d", &choice);

    if (choice >= 1 && choice <= 3) {
        if (player->gold >= skills[choice - 1].price) {
            if (player->skills[choice - 1] == 0) {
                player->skills[choice - 1] = 1;
                player->gold -= skills[choice - 1].price;
                printf("你購買了 %s!\n", skills[choice - 1].name);
            } else {
                printf("你已經擁有這個技能了!\n");
            }
        } else {
            printf("金幣不足!\n");
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
        printf("\n選擇行動:\n1. 戰鬥\n2. 商店\n3. 離開\n");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) battle(&player);
        else if (choice == 2) shop(&player);
        else break;
    }

    return 0;
}
