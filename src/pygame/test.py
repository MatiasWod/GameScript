import random

import pygame

# Dimensiones de la pantalla
WIDTH, HEIGHT = 800, 600

# Inicialización de Pygame
pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("GameScript")

clock = pygame.time.Clock()

#  Variables



# Función para dibujar elementos en la pantalla



# Bucle principal del juego
running = True
while running:
    pygame.display.flip()
    clock.tick(60)
pygame.quit()