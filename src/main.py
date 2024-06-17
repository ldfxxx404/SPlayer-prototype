import pyglet
from pyglet.window import key

class VideoPlayerWindow(pyglet.window.Window):
    def __init__(self, media_file, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.player = pyglet.media.Player()
        self.source = pyglet.media.load(media_file)
        self.player.queue(self.source)
        self.player.play()

        self.paused = False
        self.muted = False
        self.volume = 1.0
        self.speed = 1.0

    def on_draw(self):
        self.clear()
        if self.player.source:
            self.update_video_position()

    def update_video_position(self):
        texture = self.player.get_texture()
        if texture:
            video_width = texture.width
            video_height = texture.height

            window_width = self.width
            window_height = self.height

            scale_x = window_width / video_width
            scale_y = window_height / video_height
            scale_factor = min(scale_x, scale_y)

            video_x = (window_width - video_width * scale_factor) / 2
            video_y = (window_height - video_height * scale_factor) / 2

            texture.blit(video_x, video_y, width=int(video_width * scale_factor), height=int(video_height * scale_factor))

    def on_key_press(self, symbol, modifiers):
        if symbol == key.SPACE:
            if self.paused:
                self.player.play()
                self.paused = False
            else:
                self.player.pause()
                self.paused = True

        elif symbol == key.M:
            self.muted = not self.muted
            self.player.volume = 0 if self.muted else self.volume

        elif symbol == key.EQUAL:
            self.volume = min(1.0, self.volume + 0.1)
            self.player.volume = self.volume

        elif symbol == key.MINUS:
            self.volume = max(0.0, self.volume - 0.1)
            self.player.volume = self.volume

        elif symbol == key.RIGHT:
            self.speed += 0.5
            self.player.speed = self.speed

        elif symbol == key.LEFT:
            self.speed = max(0.5, self.speed - 0.5)
            self.player.speed = self.speed

        elif symbol == key.Q:
            self.player.pause()
            pyglet.app.exit()

def play_media(media_file):
    try:
        window = VideoPlayerWindow(media_file, width=800, height=600, caption='Video Player', resizable=True)
        pyglet.app.run()

    except Exception as e:
        print(f'Error playing media: {e}')

if __name__ == '__main__':
    import sys
    if len(sys.argv) < 2:
        print('Usage: python main.py <path_to_video_file>')
        sys.exit(1)

    media_file = sys.argv[1]
    play_media(media_file)
