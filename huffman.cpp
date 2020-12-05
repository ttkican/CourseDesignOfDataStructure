#include <iostream>
#include <string.h>
#define M 1000
using namespace std;
// 树结点
typedef struct
{
    char letter; // 叶子结点字符
    char *code;  // 存放叶子结点编码
    int weight;  // 权值
    int parent;  // 双亲结点
    int lchild;  // 左子结点
    int rchild;  // 右子结点
} HTNode, *HuffmanTree;
// 记录每个字符以及字符出现的次数
struct word
{
    char str;          // 字符
    int num;           // 次数
    struct word *next; // 指向下一个
};
char *a[M];       // 存放内容的哈夫曼编码
int b;            // 所有字符的哈夫曼编码个数
int *char_nums;   // 装每个字符出现的次数，也就是权值
char *zif;        // 文件中不重复的字符
int words = 1;    //文件中所有字符的个数
int leaf = 0;     // 叶子结点的个数
char contents[M]; // 文件中所有的内容
/*计算文件中每个字符出现的次数 */
void CalculateWords(struct word *head, char ch)
{
    struct word *p, *q;
    p = head; // p重新指向head
    while (1)
    {
        if (p->str == ch) // 出现相同的字符
        {
            p->num++; // 字符对应的次数加一
            break;
        }
        else // 出现新的字符
        {
            if (p->next == NULL) // 前面没有相同的字符
            {
                q = (struct word *)malloc(sizeof(struct word)); // 创建一个新的结点
                memset(q, 0, sizeof(struct word));              // 对结点进行初始化
                q->str = ch;
                q->num = 1;
                p->next = q; // 将新的字符添加到链表后面
                break;
            }
            else // 指向下一个字符
                p = p->next;
        }
    }
}
/* 选择两棵parent为0且权值最小的子树，根结点分别为s1和s2 */
void SELECT(HuffmanTree &HT, int n, int &s1, int &s2)
{
    int min_num; //保存权值最小的根结点
    /* 找到第一个权值最小的根结点 */
    for (int i = 1; i <= n; i++)
    {
        if (HT[i].parent == 0)
        {
            min_num = i;
            break;
        }
    }
    for (int i = 1; i <= n; i++)
    {
        if (HT[i].parent == 0)
            if (HT[i].weight < HT[min_num].weight)
                min_num = i;
    }
    s1 = min_num;
    /* 找到第二权值最小的根结点 */
    for (int i = 1; i <= n; i++)
    {
        if (HT[i].parent == 0 && i != s1)
        {
            min_num = i;
            break;
        }
    }
    for (int i = 1; i <= n; i++)
    {
        if (HT[i].parent == 0 && i != s1)
            if (HT[i].weight < HT[min_num].weight)
                min_num = i;
    }
    s2 = min_num;
}
/* 创建哈夫曼树 */
void CreateHuffmanTree(HuffmanTree &HT, char t[], int w[], int n)
{
    int s1, s2;
    int m = 2 * n - 1;           // 总结点为度为2的结点加上度为0的结点
    HT = new HTNode[m];          // 长度为m+1的结构体数组
    for (int i = 1; i <= n; i++) // 叶子结点初始化
    {
        HT[i].letter = t[i];
        HT[i].weight = w[i];
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
    }
    for (int i = n + 1; i <= m; i++) // 非叶子结点初始化
    {
        HT[i].letter = ' ';
        HT[i].weight = 0;
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
    }
    for (int i = n + 1; i <= m; i++) // 创建哈夫曼表
    {
        SELECT(HT, i - 1, s1, s2);
        HT[s1].parent = i;                            // 第i个结点为s1结点的双亲
        HT[s2].parent = i;                            // 第i个结点为s2结点的双亲
        HT[i].lchild = s1;                            // 第i个结点的左孩子为s1
        HT[i].rchild = s2;                            // 第i个结点的右孩子为s2
        HT[i].weight = HT[s1].weight + HT[s2].weight; // s1和s2的权值和等于双亲结点的权值
    }
}
/* 将源文件转换成哈夫曼编码 */
void TranslateSourceFile(HuffmanTree &HT, int n)
{
    FILE *fp;
    if ((fp = fopen("huffmanfile.txt", "a")) == NULL)
    {
        cout << "open file failed!" << endl;
        exit(0);
    }
    cout << "The code corresponding to each character:" << endl;
    char *code = new char[n]; //为哈弗曼编码动态分配空间
    code[n - 1] = '\0';       //如：3个结点编码最长为2。cd[3-1] = '\0';
    int start, c, f;
    for (int i = 1; i <= n; i++)
    {
        start = n - 1;
        for (c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent) // 从叶子到根结点逆向求编码
        {
            if (HT[f].lchild == c)
                code[--start] = '0';
            else
                code[--start] = '1';
        }
        //为第i个字符分配编码空间
        HT[i].code = new char[n - start];
        //将当前求出结点的哈弗曼编码复制到叶子结点编码中
        strcpy(HT[i].code, &code[start]);
        cout << HT[i].letter << ":" << HT[i].code << endl;
    }
    // 对文件中的内容进行编码
    cout << "File encoding succeeded............\n";
    for (int i = 1; i < words; i++)
        for (int j = 1; j <= n; j++)
            if (contents[i] == HT[j].letter) // 如果内容的中字符与叶子结点字符相等
            {
                a[++b] = HT[j].code;     // 将内容的中字符相应的哈夫曼编码存入数组
                fprintf(fp, HT[j].code); // 将内容的中字符相应的哈夫曼编码存入文件
            }
    fclose(fp);
    free(code);
}
/* 从源文件中读取信息 */
void ReadSourceFile()
{
    FILE *fp;
    char ch;
    struct word *head, *p;
    int front = 0; // 不重复字符的个数
    head = (struct word *)malloc(sizeof(struct word));
    memset(head, 0, sizeof(struct word)); // 对head头指针初始化
    /* 获取文件内容并将内容写入contents，words记录字符个数 */
    if ((fp = fopen("sourcefile.txt", "r")) == NULL) // 打开文件
    {
        cout << "open file failed!" << endl;
        exit(0);
    }
    while (fscanf(fp, "%c", &contents[words]) != EOF)
        words++;
    fclose(fp);
    /* 计算文件内每个字符出现的次数 */
    if ((fp = fopen("sourcefile.txt", "r")) == NULL) // 打开文件
    {
        cout << "open file failed!" << endl;
        exit(0);
    }
    while ((ch = fgetc(fp)) != EOF) // 从文件中一个个读取字符
        CalculateWords(head, ch);
    fclose(fp);
    cout << "File read successfully............\n"
         << "Character and Weights of Character" << endl;
    char_nums = new int[words]; // 根据文件中的所有字符开辟
    zif = new char[words];      // 根据文件中的所有字符个数开辟
    for (p = head->next; p != NULL; p = p->next)
    {
        printf("Character:%c, Weight:%d\n", p->str, p->num);
        zif[++front] = p->str;      // 出现的每个字符存入数组
        char_nums[++leaf] = p->num; // 每个字符的次数(权值)存入数组
    }
}
/* 对文件进行解码 */
void releaseHuffCode(HuffmanTree &HT, int n)
{
    FILE *fp;
    if ((fp = fopen("releasefile.txt", "a")) == NULL) // 打开文件
    {
        cout << "open file failed!" << endl;
        exit(0);
    }
    for (int i = 1; i <= b; i++)
        for (int j = 1; j <= n; j++) // 将内容中的每个哈夫曼编码与结构体中的每个字符的哈夫曼编码对比
            if (a[i] == HT[j].code)
                fprintf(fp, "%c", HT[j].letter);
    cout << "The file was decoded successfully............\n";
}
int main()
{
    HuffmanTree HT = NULL;
    ReadSourceFile();
    CreateHuffmanTree(HT, zif, char_nums, leaf);
    TranslateSourceFile(HT, leaf);
    releaseHuffCode(HT, leaf);
    system("pause");
    return 0;
}