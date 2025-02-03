#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 角色結構
typedef struct {
    char name[20];  // 角色名稱
    int hp;         // 生命值
    int max_hp;     // 最大生命值
    int atk;        // 攻擊力
    int def;        // 防禦力
    int exp;        // 經驗值
    int level;      // 等級
} Character;

// 敵人結構
typedef struct {
    char name[20];  // 敵人名稱
    int hp;         // 生命值
    int atk;        // 攻擊力
    int def;        // 防禦力
    int exp;        // 提供的經驗值
} Enemy;

// 初始化玩家角色
void initPlayer(Character *player) {
    printf("請輸入你的角色名稱: ");
    scanf("%s", player->name);
    player->level = 1;
    player->max_hp = 100;
    player->hp = player->max_hp;
    player->atk = 15;
    player->def = 10;
    player->exp = 0;
}

// 初始化敵人
void initEnemy(Enemy *enemy, int level) {
    sprintf(enemy->name, "敵人 Lv.%d", level);
    enemy->hp = 50 + (level * 10);
    enemy->atk = 10 + (level * 5);
    enemy->def = 5 + (level * 3);
    enemy->exp = 20 + (level * 5);
}

// 顯示角色狀態
void showStatus(Character *player) {
    printf("\n=== 角色狀態 ===\n");
    printf("名稱: %s\n", player->name);
    printf("等級: %d\n", player->level);
    printf("生命值: %d/%d\n", player->hp, player->max_hp);
    printf("攻擊力: %d\n", player->atk);
    printf("防禦力: %d\n", player->def);
    printf("經驗值: %d/100\n", player->exp);
    printf("================\n");
}

// 玩家選擇招式
int chooseAction() {
    int choice;
    printf("\n選擇招式:\n");
    printf("1. 普通攻擊 (造成 100%% 傷害)\n");
    printf("2. 強力攻擊 (造成 150%% 傷害，命中率 80%%)\n");
    printf("3. 治療 (回復 20%% 最大生命值)\n");
    printf("請輸入選擇: ");
    scanf("%d", &choice);
    return choice;
}

// 敵人選擇行動
int enemyAction() {
    return rand() % 2 + 1;  // 1: 攻擊, 2: 防禦
}

// 戰鬥系統
void battle(Character *player, Enemy *enemy) {
    printf("\n=== 戰鬥開始 ===\n");
    printf("%s 對上了 %s！\n", player->name, enemy->name);

    while (player->hp > 0 && enemy->hp > 0) {
        // 玩家回合
        showStatus(player);
        int action = chooseAction();
        int damage;

        switch (action) {
            case 1:  // 普通攻擊
                damage = player->atk - enemy->def;
                if (damage < 0) damage = 0;
                enemy->hp -= damage;
                printf("%s 使用普通攻擊，對 %s 造成了 %d 點傷害！\n", player->name, enemy->name, damage);
                break;

            case 2:  // 強力攻擊
                if (rand() % 100 < 80) {  // 80% 命中率
                    damage = (int)(player->atk * 1.5) - enemy->def;
                    if (damage < 0) damage = 0;
                    enemy->hp -= damage;
                    printf("%s 使用強力攻擊，對 %s 造成了 %d 點傷害！\n", player->name, enemy->name, damage);
                } else {
                    printf("%s 的強力攻擊未命中！\n", player->name);
                }
                break;

            case 3:  // 治療
                damage = (int)(player->max_hp * 0.2);
                player->hp += damage;
                if (player->hp > player->max_hp) player->hp = player->max_hp;
                printf("%s 使用治療，回復了 %d 點生命值！\n", player->name, damage);
                break;

            default:
                printf("無效選擇，跳過本回合。\n");
                break;
        }

        if (enemy->hp <= 0) {
            printf("%s 被擊敗了！\n", enemy->name);
            break;
        }

        // 敵人回合
        int enemyActionChoice = enemyAction();
        if (enemyActionChoice == 1) {  // 敵人攻擊
            damage = enemy->atk - player->def;
            if (damage < 0) damage = 0;
            player->hp -= damage;
            printf("%s 對 %s 造成了 %d 點傷害！\n", enemy->name, player->name, damage);
        } else {  // 敵人防禦
            enemy->def += 5;
            printf("%s 進行防禦，防禦力提升 5 點！\n", enemy->name);
        }

        if (player->hp <= 0) {
            printf("%s 被擊敗了...\n", player->name);
            break;
        }
    }

    if (player->hp > 0) {
        printf("\n戰鬥勝利！\n");
        player->exp += enemy->exp;
        printf("獲得 %d 點經驗值！\n", enemy->exp);

        // 檢查是否升級
        if (player->exp >= 100) {
            player->level++;
            player->max_hp += 20;
            player->hp = player->max_hp;
            player->atk += 5;
            player->def += 3;
            player->exp -= 100;
            printf("%s 升級到 Lv.%d！\n", player->name, player->level);
        }
    } else {
        printf("\n遊戲結束...\n");
        exit(0);  // 遊戲結束
    }
}

// 主遊戲循環
int main() {
    srand(time(NULL));  // 初始化隨機數種子
    Character player;
    initPlayer(&player);

    int choice;
    while (1) {
        showStatus(&player);
        printf("\n選擇行動:\n");
        printf("1. 戰鬥\n");
        printf("2. 退出遊戲\n");
        printf("請輸入選擇: ");
        scanf("%d", &choice);

        if (choice == 1) {
            Enemy enemy;
            initEnemy(&enemy, player.level);
            battle(&player, &enemy);
        } else if (choice == 2) {
            printf("退出遊戲。\n");
            break;
        } else {
            printf("無效選擇，請重新輸入。\n");
        }
    }

    return 0;
}
