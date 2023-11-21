import random

import pygame

# Dimensiones de la pantalla
WIDTH, HEIGHT = 800, 600
# Colores
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)

# Inicialización de Pygame
pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("GameScript")

# Variables
ball_speed_x = 7 * random.choice((1, -1))
ball_speed_y = 7 * random.choice((1, -1))
ball_radius = 10
ball_pos = [WIDTH // 2, HEIGHT // 2]

paddle_width = 10
paddle_height = 100
paddle_speed = 0
paddle_pos = [0, HEIGHT // 2 - paddle_height // 2]

opponent_pos = [WIDTH - paddle_width, HEIGHT // 2 - paddle_height // 2]
opponent_speed = 7

score_player = 0
score_opponent = 0
font = pygame.font.Font(None, 50)

clock = pygame.time.Clock()

# Función para dibujar elementos en la pantalla
def draw_elements():
    screen.fill(BLACK)
    pygame.draw.rect(screen, WHITE, pygame.Rect(paddle_pos[0], paddle_pos[1], paddle_width, paddle_height))
    pygame.draw.rect(screen, WHITE, pygame.Rect(opponent_pos[0], opponent_pos[1], paddle_width, paddle_height))
    pygame.draw.circle(screen, WHITE, ball_pos, ball_radius)
    score_display = font.render(f"{score_player} - {score_opponent}", True, WHITE)
    screen.blit(score_display, (WIDTH // 2 - score_display.get_width() // 2, 50))

# Bucle principal del juego
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_UP:
                paddle_speed = -7
            elif event.key == pygame.K_DOWN:
                paddle_speed = 7
        elif event.type == pygame.KEYUP:
            if event.key == pygame.K_UP or event.key == pygame.K_DOWN:
                paddle_speed = 0

    # Movimiento de la raqueta del jugador
    paddle_pos[1] += paddle_speed
    if paddle_pos[1] < 0:
        paddle_pos[1] = 0
    elif paddle_pos[1] > HEIGHT - paddle_height:
        paddle_pos[1] = HEIGHT - paddle_height

    # Movimiento de la raqueta del oponente
    if ball_pos[1] - paddle_width // 2 > opponent_pos[1] + paddle_height // 2:
        opponent_pos[1] += opponent_speed
    elif ball_pos[1] + paddle_width // 2 < opponent_pos[1] + paddle_height // 2:
        opponent_pos[1] -= opponent_speed

    # Movimiento de la pelota
    ball_pos[0] += ball_speed_x
    ball_pos[1] += ball_speed_y

    # Colisiones con las paredes
    if ball_pos[1] <= 0 or ball_pos[1] >= HEIGHT - ball_radius:
        ball_speed_y = -ball_speed_y

    # Colisiones con las raquetas
    if ball_pos[0] <= paddle_pos[0] + paddle_width and ball_pos[1] >= paddle_pos[1] and ball_pos[1] <= paddle_pos[1] + paddle_height:
        ball_speed_x = -ball_speed_x
    elif ball_pos[0] >= opponent_pos[0] - ball_radius and ball_pos[1] >= opponent_pos[1] and ball_pos[1] <= opponent_pos[1] + paddle_height:
        ball_speed_x = -ball_speed_x

    # Puntuación
    if ball_pos[0] < 0:
        score_opponent += 1
        ball_pos = [WIDTH // 2, HEIGHT // 2]
        ball_speed_x *= random.choice((1, -1))
        ball_speed_y *= random.choice((1, -1))
    elif ball_pos[0] > WIDTH:
        score_player += 1
        ball_pos = [WIDTH // 2, HEIGHT // 2]
        ball_speed_x *= random.choice((1, -1))
        ball_speed_y *= random.choice((1, -1))

    # Dibujar elementos en la pantalla
    draw_elements()

    # Actualizar la pantalla
    pygame.display.flip()
    clock.tick(60)

pygame.quit()
