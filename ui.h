#ifndef UI_H
#define UI_H

/*
 * ui.h — Terminal UI declarations for OS.Kill()
 * Semua fungsi yang berhubungan dengan tampilan terminal dikumpulkan di sini.
 */

/*
 * drawManual — tampilkan panduan perintah sesuai wave saat ini.
 *   wave : wave yang sedang berjalan (1, 2, atau 3)
 */
void drawManual(int wave);

/*
 * drawUI — clear layar lalu render seluruh tampilan HUD.
 *   loc  : string lokasi saat ini, misal "C:/Root/System"
 *   wave : wave saat ini
 *   vHP  : HP virus
 *   pHP  : HP player
 *   msg  : pesan log yang ditampilkan di baris bawah
 */
void drawUI(char *loc, int wave, int vHP, int pHP, char *msg);

/*
 * drawGameOver — tampilkan layar kalah.
 */
void drawGameOver(void);

/*
 * drawVictory — tampilkan layar menang, tunggu Enter.
 */
void drawVictory(void);

#endif /* UI_H */
