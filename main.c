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

typedef struct
{
    Vector3 color;
    Vector3 emision;
    float albedo;
    float especular;
    float brillo;
    float reflectividad;
} Materiales;

typedef struct
{
    BoundingBox caja;
    Materiales material;
} Cubo;

void cameraLook(CameraRay *camera, Vector3 target)
{
    camera->forward = Vector3Normalize(Vector3Subtract(target, camera->position));
    camera->right = Vector3Normalize(Vector3CrossProduct(camera->forward, (Vector3){0, 1, 0}));
    camera->up = Vector3Normalize(Vector3CrossProduct(camera->right, camera->forward));
}

Color vectorColor(Vector3 color)
{
    unsigned char r = (unsigned char)(sqrt(Clamp(color.x, 0, 1)) * 255);
    unsigned char g = (unsigned char)(sqrt(Clamp(color.y, 0, 1)) * 255);
    unsigned char b = (unsigned char)(sqrt(Clamp(color.z, 0, 1)) * 255);
    return (Color){r, g, b, 255};
}

Cubo cubosDiorama[10];
int numCubos = 0;
Vector3 posLuz = (Vector3){3, 5, 4};
Model modelo;
float escalaModelo = 0.08f;
Matrix transformModelo;
BoundingBox cajaModelo;
Materiales materialModelo;

RayCollision colisionRayo(Vector3 origen, Vector3 direccion, int *indice)
{
    RayCollision masCercano = {0};
    masCercano.distance = 1000.0f;
    for (int i = 0; i < numCubos; i++)
    {
        RayCollision colision = GetRayCollisionBox((Ray){origen, direccion}, cubosDiorama[i].caja);
        if (colision.hit && colision.distance > 0.001f && colision.distance < masCercano.distance)
        {
            masCercano = colision;
            *indice = i;
        }
    }
    Ray rayo = {origen, direccion};
    RayCollision colisionCaja = GetRayCollisionBox(rayo, cajaModelo);
    if (colisionCaja.hit)
    {
        RayCollision colision = GetRayCollisionMesh(rayo, modelo.meshes[0], transformModelo);
        if (colision.hit && colision.distance > 0.001f && colision.distance < masCercano.distance)
        {
            masCercano = colision;
            *indice = -1;
        }
    }
    return masCercano;
}

Vector3 cielo(Vector3 direccion)
{
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

Vector3 castRay(Vector3 origen, Vector3 direccion)
{
    int indice = 0;
    RayCollision choque = colisionRayo(origen, direccion, &indice);
    if (!choque.hit)
    {
        return cielo(direccion);
    }
    Materiales material;
    if (indice == -1)
    {
        material = materialModelo;
    }
    else
    {
        material = cubosDiorama[indice].material;
    }
    Vector3 dirLuz = Vector3Normalize(Vector3Subtract(posLuz, choque.point));
    float difusa = fmaxf(0, Vector3DotProduct(choque.normal, dirLuz));
    Vector3 reflejo = Vector3Reflect(Vector3Negate(dirLuz), choque.normal);
    float spec = powf(fmaxf(0, Vector3DotProduct(reflejo, Vector3Negate(direccion))), material.brillo);
    Vector3 origenSombra = Vector3Add(choque.point, Vector3Scale(choque.normal, 0.001f));
    float distLuz = Vector3Length(Vector3Subtract(posLuz, choque.point));
    int indiceSombra = 0;
    RayCollision sombra = colisionRayo(origenSombra, dirLuz, &indiceSombra);
    if (sombra.hit && sombra.distance < distLuz)
    {
        difusa = 0;
        spec = 0;
    }
    Vector3 color = Vector3Scale(material.color, 0.25f + material.albedo * difusa);
    color = Vector3Add(color, Vector3Scale((Vector3){1, 1, 1}, material.especular * spec));
    color = Vector3Add(color, material.emision);
    return color;
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

    float anguloHorizontal = 0;
    float anguloVertical = 0;
    float distancia = 6;

    // 1 Este es el piso
    cubosDiorama[numCubos].caja = (BoundingBox){{-3, -1.2f, -3}, {3, -1, 3}};
    cubosDiorama[numCubos].material = (Materiales){.color = {0.50f, 0.62f, 0.85f}, .albedo = 0.9f, .especular = 0.1f, .brillo = 10};
    numCubos++;
    // 2
    cubosDiorama[numCubos].caja = (BoundingBox){{-0.5f, -1, -0.5f}, {0.5f, 0, 0.5f}};
    cubosDiorama[numCubos].material = (Materiales){.color = {0.27f, 0.47f, 0.92f}, .albedo = 0.8f, .especular = 0.5f, .brillo = 32};
    numCubos++;
    // 3
    cubosDiorama[numCubos].caja = (BoundingBox){{-2, -1, -0.5f}, {-1, 0, 0.5f}};
    cubosDiorama[numCubos].material = (Materiales){.color = {0.22f, 0.67f, 0.89f}, .albedo = 0.8f, .especular = 0.3f, .brillo = 16};
    numCubos++;
    // 4
    cubosDiorama[numCubos].caja = (BoundingBox){{1, -1, -0.5f}, {2, 0, 0.5f}};
    cubosDiorama[numCubos].material = (Materiales){.color = {0.41f, 0.81f, 1.0f}, .albedo = 0.8f, .especular = 0.3f, .brillo = 16};
    numCubos++;
    // MSN
    modelo = LoadModel("munecoMSN.obj");
    transformModelo = MatrixMultiply(MatrixScale(escalaModelo, escalaModelo, escalaModelo), MatrixTranslate(0, 0.37f, 0));
    cajaModelo = GetMeshBoundingBox(modelo.meshes[0]);
    cajaModelo.min = Vector3Add(Vector3Scale(cajaModelo.min, escalaModelo), (Vector3){0, 0.37f, 0});
    cajaModelo.max = Vector3Add(Vector3Scale(cajaModelo.max, escalaModelo), (Vector3){0, 0.37f, 0});
    materialModelo = (Materiales){.color = {0.4f, 0.9f, 1.0f}, .emision = {0.4f, 0.9f, 1.0f}, .albedo = 0.2f};

    while (!WindowShouldClose())
    {

        float delta = GetFrameTime();
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        {
            anguloHorizontal -= 1.5f * delta;
        }
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        {
            anguloHorizontal += 1.5f * delta;
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        {
            anguloVertical += 1.5f * delta;
        }
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        {
            anguloVertical -= 1.5f * delta;
        }

        anguloVertical = Clamp(anguloVertical, -1.4f, 1.4f);
        distancia -= GetMouseWheelMove() * 0.5f;
        distancia = Clamp(distancia, 2, 20);

        cam.position = (Vector3){distancia * cosf(anguloVertical) * sinf(anguloHorizontal), distancia * sinf(anguloVertical), distancia * cosf(anguloVertical) * cosf(anguloHorizontal)};
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
    UnloadModel(modelo);
    CloseWindow();
    return 0;
}