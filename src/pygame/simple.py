import random

import pygame

# Inicialización de Pygame
pygame.init()

# Dimensiones de la pantalla
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Click Game")

# Colores
colors = [(255, 0, 0), (0, 255, 0), (0, 0, 255), (255, 255, 0), (255, 0, 255), (0, 255, 255)]

# Función para dibujar un punto de color aleatorio en la pantalla en la posición del clic
def draw_point(pos):
    color = random.choice(colors)
    pygame.draw.circle(screen, color, pos, 10)

# Bucle principal del juego
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            draw_point(event.pos)

    # Actualizar la pantalla
    pygame.display.flip()

pygame.quit()
