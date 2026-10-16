/* Ventana del proyecto*/

#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdio.h>

#define ANCHO 320
#define ALTO 180
#define PIXELES (ANCHO * ALTO)
#define ESCALA 4
Color colores[PIXELES];

void printVector(Vector3 vector)
{
    printf("x: %f, y: %f, z: %f\n", vector.x, vector.y, vector.z);
}

typedef struct
{
    Vector3 position;
    Vector3 forward;
    Vector3 right;
    Vector3 up;
} CameraRay;

void cameraLook(CameraRay *camera, Vector3 target)
{
    camera->forward = Vector3Normalize(Vector3Subtract(target, camera->position));
    camera->right = Vector3Normalize(Vector3CrossProduct(camera->forward, (Vector3){0, 1, 0}));
    camera->up = Vector3Normalize(Vector3CrossProduct(camera->right, camera->forward));
}

Color vectorColor(Vector3 color)
{
    unsigned char r = (unsigned char)(Clamp(color.x, 0, 1) * 255);
    unsigned char g = (unsigned char)(Clamp(color.y, 0, 1) * 255);
    unsigned char b = (unsigned char)(Clamp(color.z, 0, 1) * 255);
    return (Color){r, g, b, 255};
}

Vector3 castRay(Vector3 origen, Vector3 direccion)
{
    (void)origen;
    Vector3 horizonte = (Vector3){0.55f, 0.85f, 0.3f};
    Vector3 gramaProfundidad = (Vector3){0.1f, 0.5f, 0.15f};
    if (direccion.y < 0)
    {
        return Vector3Lerp(horizonte, gramaProfundidad, -direccion.y);
    }
    else
    {
        return (Vector3Lerp((Vector3){1, 1, 1}, (Vector3){0.4f, 0.6f, 1.0f}, direccion.y));
    }
}

int main(void)
{
    const int screenWidth = ANCHO * ESCALA;
    const int screenHeight = ALTO * ESCALA;

    InitWindow(screenWidth, screenHeight, "Proyecto 2 Raytracing");

    SetTargetFPS(60);

    // La textura que voy a pintar
    Image imagenNegro = GenImageColor(ANCHO, ALTO, BLACK);
    Texture2D texturaColores = LoadTextureFromImage(imagenNegro);
    UnloadImage(imagenNegro);

    CameraRay cam = {.position = {0, 1, 6}};
    float mitad = tanf(60 * DEG2RAD / 2);
    float aspecto = (float)ANCHO / ALTO;

    while (!WindowShouldClose())
    {
        cameraLook(&cam, (Vector3){0, 0, 0});

        // Arreglo de colores
        for (int y = 0; y < ALTO; y++)
        {
            for (int x = 0; x < ANCHO; x++)
            {
                int indice = ((y * ANCHO) + x);
                float pantallax = ((x + 0.5f) / ANCHO * 2 - 1) * aspecto * mitad;
                float pantallay = (1 - (y + 0.5f) / ALTO * 2) * mitad;
                Vector3 direccion = Vector3Normalize(Vector3Add(Vector3Add(cam.forward, Vector3Scale(cam.right, pantallax)), Vector3Scale(cam.up, pantallay)));
                colores[indice] = vectorColor(castRay(cam.position, direccion));
            }
        }
        // Lo paso a la textura
        UpdateTexture(texturaColores, colores);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureEx(texturaColores, (Vector2){0, 0}, 0, ESCALA, WHITE);
        DrawFPS(5, 5);
        EndDrawing();
    }

    UnloadTexture(texturaColores);
    CloseWindow();
    return 0;
}