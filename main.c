/* Ventana del proyecto*/

#include "raylib.h"
#include <stdio.h>

#define ANCHO 320
#define ALTO 180
#define PIXELES (ANCHO * ALTO)
#define ESCALA 4
Color colores[PIXELES];

int main(void)
{
    const int screenWidth = ANCHO * ESCALA;
    const int screenHeight = ALTO * ESCALA;
    const char textInicio[] = "Setup de proyecto";
    const int fontSize = 15;

    InitWindow(screenWidth, screenHeight, "Proyecto 2 Raytracing");

    // Para centrar el texto
    int measurement = MeasureText(textInicio, fontSize);

    SetTargetFPS(60);

    // La textura que voy a pintar
    Image imagenNegro = GenImageColor(ANCHO, ALTO, BLACK);
    Texture2D texturaColores = LoadTextureFromImage(imagenNegro);
    UnloadImage(imagenNegro);

    while (!WindowShouldClose())
    {
        // Arreglo de colores
        for (int y = 0; y < ALTO; y++)
        {
            for (int x = 0; x < ANCHO; x++)
            {
                int indice = ((y * ANCHO) + x);
                float rojo = (float)x / (ANCHO - 1) * 255;
                float verde = (float)y / (ALTO - 1) * 255;
                colores[indice] = (Color){(unsigned char)rojo, (unsigned char)verde, 128, 255};
            }
        }
        // Lo paso a la textura
        UpdateTexture(texturaColores, colores);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureEx(texturaColores, (Vector2){0, 0}, 0, ESCALA, WHITE);
        DrawFPS(5, 5);
        DrawText(textInicio, (screenWidth / 2) - (measurement / 2), (screenHeight / 2) - (fontSize / 2), fontSize, RAYWHITE);
        EndDrawing();
    }

    UnloadTexture(texturaColores);
    CloseWindow();
    return 0;
}