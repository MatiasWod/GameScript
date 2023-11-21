import sys

import pygame

# Inicialización de Pygame
pygame.init()

# Dimensiones de la pantalla
WIDTH, HEIGHT = 300, 300
LINE_WIDTH = 15
BOARD_ROWS, BOARD_COLS = 3, 3
SQUARE_SIZE = WIDTH // BOARD_COLS

# Colores
WHITE = (255, 255, 255)
LINE_COLOR = (23, 145, 135)
BG_COLOR = (28, 170, 156)
CIRCLE_COLOR = (239, 231, 200)
CROSS_COLOR = (66, 66, 66)

# Ventana del juego
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Tic Tac Toe")
screen.fill(BG_COLOR)

# Tablero del juego
board = [['' for _ in range(BOARD_COLS)] for _ in range(BOARD_ROWS)]

# Función para dibujar el tablero
def draw_board():
    for row in range(1, BOARD_ROWS):
        pygame.draw.line(screen, LINE_COLOR, (0, row * SQUARE_SIZE), (WIDTH, row * SQUARE_SIZE), LINE_WIDTH)
    for col in range(1, BOARD_COLS):
        pygame.draw.line(screen, LINE_COLOR, (col * SQUARE_SIZE, 0), (col * SQUARE_SIZE, HEIGHT), LINE_WIDTH)

# Función para dibujar la X y el círculo
def draw_figures():
    for row in range(BOARD_ROWS):
        for col in range(BOARD_COLS):
            if board[row][col] == 'X':
                pygame.draw.line(screen, CROSS_COLOR, (col * SQUARE_SIZE + 20, row * SQUARE_SIZE + 20),
                                 ((col + 1) * SQUARE_SIZE - 20, (row + 1) * SQUARE_SIZE - 20), LINE_WIDTH)
                pygame.draw.line(screen, CROSS_COLOR, ((col + 1) * SQUARE_SIZE - 20, row * SQUARE_SIZE + 20),
                                 (col * SQUARE_SIZE + 20, (row + 1) * SQUARE_SIZE - 20), LINE_WIDTH)
            elif board[row][col] == 'O':
                pygame.draw.circle(screen, CIRCLE_COLOR, (int(col * SQUARE_SIZE + SQUARE_SIZE / 2),
                                                          int(row * SQUARE_SIZE + SQUARE_SIZE / 2)),
                                   int(SQUARE_SIZE / 2 - 20), LINE_WIDTH)

# Función para marcar la casilla seleccionada
def mark_square(row, col, player):
    board[row][col] = player

# Función para verificar el estado del juego
def check_win(player):
    # Verificar filas y columnas
    for row in range(BOARD_ROWS):
        if all([board[row][col] == player for col in range(BOARD_COLS)]) or \
           all([board[col][row] == player for col in range(BOARD_COLS)]):
            return True

    # Verificar diagonales
    if all([board[i][i] == player for i in range(BOARD_ROWS)]) or \
       all([board[i][BOARD_COLS - 1 - i] == player for i in range(BOARD_ROWS)]):
        return True

    return False

# Función principal del juego
def play_game():
    player = 'X'
    game_over = False

    while not game_over:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

            if event.type == pygame.MOUSEBUTTONDOWN:
                mouseX = event.pos[0]
                mouseY = event.pos[1]

                clicked_row = int(mouseY // SQUARE_SIZE)
                clicked_col = int(mouseX // SQUARE_SIZE)

                if board[clicked_row][clicked_col] == '':
                    mark_square(clicked_row, clicked_col, player)
                    if check_win(player):
                        game_over = True
                    if player == 'X':
                        player = 'O'
                    else:
                        player = 'X'

        screen.fill(BG_COLOR)
        draw_board()
        draw_figures()
        pygame.display.update()

        if game_over:
            pygame.time.wait(3000)
            board.clear()
            board.extend([['' for _ in range(BOARD_COLS)] for _ in range(BOARD_ROWS)])
            screen.fill(BG_COLOR)
            draw_board()
            draw_figures()
            pygame.display.update()
            game_over = False

# Iniciar el juego
play_game()
