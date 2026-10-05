"""
Генератор тестового спрайт-листа персонажа.
Создаёт PNG 4 направления x 4 кадра = 16 кадров размером 32x32.

Каждый кадр — квадрат с цветом, зависящим от направления,
и "стрелкой-указателем" направления. Плюс лёгкое покачивание
размера квадрата, чтобы анимация была видна.
"""

from PIL import Image, ImageDraw
from pathlib import Path

# ===== Параметры =====
FRAME_SIZE  = 32
FRAMES_PER_DIR = 4
DIRECTIONS  = ["down", "left", "right", "up"]  # порядок строк в листе

# Цвет квадрата для каждого направления
COLORS = {
    "down":  (220, 80, 80),    # красный
    "left":  (80, 140, 220),   # синий
    "right": (80, 200, 100),   # зелёный
    "up":    (200, 180, 80),   # жёлтый
}

OUT_PATH = Path(__file__).resolve().parent.parent / "assets" / "textures" / "player_sheet.png"
OUT_PATH.parent.mkdir(parents=True, exist_ok=True)


def draw_frame(draw: ImageDraw.ImageDraw,
               x: int, y: int,
               direction: str,
               frame_index: int):
    """Рисует один кадр в точке (x, y)."""
    color = COLORS[direction]

    # Лёгкое "дыхание" — квадрат чуть-чуть сжимается/расширяется по кадрам.
    # Это делает анимацию визуально заметной.
    inset = [3, 5, 3, 1][frame_index]
    box = (x + inset, y + inset,
           x + FRAME_SIZE - inset - 1, y + FRAME_SIZE - inset - 1)

    draw.rectangle(box, fill=color, outline=(255, 255, 255))

    # Указатель направления — маленький маркер в центре
    cx, cy = x + FRAME_SIZE // 2, y + FRAME_SIZE // 2
    marker = 3 + frame_index  # растёт от кадра к кадру — видно, что анимация идёт

    if direction == "down":
        draw.polygon([(cx, cy + marker),
                      (cx - marker, cy - marker // 2),
                      (cx + marker, cy - marker // 2)], fill=(255, 255, 255))
    elif direction == "up":
        draw.polygon([(cx, cy - marker),
                      (cx - marker, cy + marker // 2),
                      (cx + marker, cy + marker // 2)], fill=(255, 255, 255))
    elif direction == "left":
        draw.polygon([(cx - marker, cy),
                      (cx + marker // 2, cy - marker),
                      (cx + marker // 2, cy + marker)], fill=(255, 255, 255))
    elif direction == "right":
        draw.polygon([(cx + marker, cy),
                      (cx - marker // 2, cy - marker),
                      (cx - marker // 2, cy + marker)], fill=(255, 255, 255))


def main():
    sheet_w = FRAME_SIZE * FRAMES_PER_DIR
    sheet_h = FRAME_SIZE * len(DIRECTIONS)

    # RGBA — с альфа-каналом, чтобы фон был прозрачным
    img = Image.new("RGBA", (sheet_w, sheet_h), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)

    for row, direction in enumerate(DIRECTIONS):
        for col in range(FRAMES_PER_DIR):
            draw_frame(draw, col * FRAME_SIZE, row * FRAME_SIZE, direction, col)

    img.save(OUT_PATH)
    print(f"Saved: {OUT_PATH}")
    print(f"Size:  {sheet_w}x{sheet_h}  ({FRAMES_PER_DIR} frames x {len(DIRECTIONS)} directions)")
    print(f"Frame: {FRAME_SIZE}x{FRAME_SIZE}")


if __name__ == "__main__":
    main()