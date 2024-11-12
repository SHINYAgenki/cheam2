#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PDF_PATH "con2_latter_2024_protected.pdf"  // 対象のPDFファイルパス
#define OUTPUT_PATH "output_unlocked.pdf"          // 出力ファイルのパス

// PDFを解除する関数
int unlock_pdf(const char *password) {
    char command[256];
    snprintf(command, sizeof(command), "qpdf --password=%s --decrypt %s %s", password, PDF_PATH, OUTPUT_PATH);
    int result = system(command);
    return result;  // 0の場合は解除成功、それ以外は失敗
}

int main() {
    const char *password = "5LVk7v8HLM6nYc";  // 既知のパスワードを設定

    printf("Trying password: %s\n", password);
    if (unlock_pdf(password) == 0) {
        printf("PDF unlocked successfully with password: %s\n", password);
        return 0;  // 成功時に終了
    } else {
        printf("Failed to unlock PDF with password: %s\n", password);
        return 1;  // 解除失敗時
    }
}
