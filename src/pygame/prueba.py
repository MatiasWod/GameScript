import random

import pygame

# Inicialización de Pygame
pygame.init()

# Dimensiones de la pantalla
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Catch Game")

# Colores
WHITE = (255, 255, 255)
RED = (255, 0, 0)

# Jugador
player_width, player_height = 50, 50
player_pos = [WIDTH // 2 - player_width // 2, HEIGHT - player_height - 10]
player_speed = 10

# Objeto a atrapar
object_width, object_height = 50, 50
object_pos = [random.randint(0, WIDTH - object_width), 0]
object_speed = 5

# Otros parámetros
score = 0
clock = pygame.time.Clock()
font = pygame.font.Font(None, 36)

# Función para dibujar elementos en la pantalla
def draw_elements():
    screen.fill(WHITE)
    pygame.draw.rect(screen, RED, (player_pos[0], player_pos[1], player_width, player_height))
    pygame.draw.rect(screen, RED, (object_pos[0], object_pos[1], object_width, object_height))
    score_display = font.render(f"Score: {score}", True, (0, 0, 0))
    screen.blit(score_display, (10, 10))

# Bucle principal del juego
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    keys = pygame.key.get_pressed()
    if keys[pygame.K_LEFT]:
        player_pos[0] -= player_speed
    if keys[pygame.K_RIGHT]:
        player_pos[0] += player_speed

    # Movimiento del objeto
    object_pos[1] += object_speed
    if object_pos[1] > HEIGHT:
        object_pos[1] = 0
        object_pos[0] = random.randint(0, WIDTH - object_width)
        score += 1

    # Verificación de colisión
    if (player_pos[1] < object_pos[1] + object_height and
            player_pos[1] + player_height > object_pos[1] and
            player_pos[0] < object_pos[0] + object_width and
            player_pos[0] + player_width > object_pos[0]):
        object_pos[1] = 0
        object_pos[0] = random.randint(0, WIDTH - object_width)
        score += 1

    # Dibujar elementos en la pantalla
    draw_elements()

    # Actualizar la pantalla
    pygame.display.flip()
    clock.tick(60)

pygame.quit()
