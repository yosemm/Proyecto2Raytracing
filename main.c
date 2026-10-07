/* Ventana del proyecto*/

#include "raylib.h"
#include <stdio.h>

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;
    const char textInicio[] = "Setup de proyecto";
    const int fontSize = 15;

    InitWindow(screenWidth, screenHeight, "Proyecto 2 Raytracing");

    // Para centrar el texto
    int measurement = MeasureText(textInicio, fontSize);
    printf("El tamaño del texto es %d \n", measurement);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(5, 5);
        DrawText(textInicio, (screenWidth / 2) - (measurement / 2), (screenHeight / 2) - (fontSize / 2), fontSize, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}