import pyglet
from pyglet.window import key
from pytube import YouTube
import os

class VideoPlayerWindow(pyglet.window.Window):
    def __init__(self, media_source, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.player = pyglet.media.Player()
        self.load_media(media_source)
        self.player.play()

        self.paused = False
        self.muted = False
        self.volume = 1.0
        self.speed = 1.0

    def load_media(self, media_source):
        if media_source.startswith('http'):
            # Загрузка видео с YouTube
            yt = YouTube(media_source)
            stream = yt.streams.filter(progressive=True, file_extension='mp4').first()
            if stream:
                video_file = stream.download()
                self.source = pyglet.media.load(video_file)
                os.remove(video_file)  # Удаляем временный файл после загрузки
            else:
                raise ValueError('YouTube video stream not found')
        else:
            # Загрузка локального видео файла
            self.source = pyglet.media.load(media_source)

        self.player.queue(self.source)

    def on_draw(self):
        pyglet.gl.glClearColor(167/255, 255/255, 131/255, 1.0)  # Задаем цвет фона (зеленый)
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
            self.toggle_pause()

        elif symbol == key.M:
            self.toggle_mute()

        elif symbol == key.EQUAL:
            self.adjust_volume(0.1)

        elif symbol == key.MINUS:
            self.adjust_volume(-0.1)

        elif symbol == key.RIGHT:
            self.adjust_speed(0.5)

        elif symbol == key.LEFT:
            self.adjust_speed(-0.5)

        elif symbol == key.Q:
            self.close()

    def toggle_pause(self):
        if self.paused:
            self.player.play()
        else:
            self.player.pause()
        self.paused = not self.paused

    def toggle_mute(self):
        self.muted = not self.muted
        self.player.volume = 0 if self.muted else self.volume

    def adjust_volume(self, delta):
        self.volume = max(0.0, min(1.0, self.volume + delta))
        self.player.volume = self.volume

    def adjust_speed(self, delta):
        self.speed = max(0.5, self.speed + delta)
        self.player.speed = self.speed

    def close(self):
        self.player.pause()
        pyglet.app.exit()

def play_media(media_source):
    try:
        window = VideoPlayerWindow(media_source, width=800, height=600, caption='Video Player', resizable=True)
        pyglet.app.run()

    except Exception as e:
        print(f'Error playing media: {e}')

if __name__ == '__main__':
    import sys
    if len(sys.argv) < 2:
        print('Usage: python main.py <path_to_video_file_or_youtube_url>')
        sys.exit(1)

    media_source = sys.argv[1]
    play_media(media_source)
