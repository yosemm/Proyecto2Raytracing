# Proyecto 2 de Gráficas por Computadora: Raytracing

## Video explicando mi proyecto:
- https://youtu.be/1IHg_xaVWMI

## Diorama funcionando:
https://github.com/user-attachments/assets/f40fddc5-1dfb-4d33-89a4-053322817230

Me inspiré un poco en la estética de Frutiger Aero, y traté de crear un modelo de Blender del muñeco que usaba MSN. Está hecho en C, usando la librería de Raylib. El modelo del muñeco lo exporté de Blender como .obj y así lo utilicé en el diorama. 

### Controles para mover la camara:

- W/S o tecla Arriba/Abajo: Sube o baja la vista
- A/D o tecla Izquierda/Derecha: Girar 
- Rueda del mouse: Hacer zoom

### Comando para compilar el proyecto: 
```gcc main.c -o diorama -O2 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11```

Luego: 
```./diorama```
