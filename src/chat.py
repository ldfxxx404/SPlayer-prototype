from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.firefox.service import Service as FirefoxService
from selenium.webdriver.firefox.options import Options as FirefoxOptions
import time

def send_message_with_captcha(url, message):

    driver_path = "/home/lxmxr/Git/SPlayer-prototype/venv/geckodriver"
    
    service = FirefoxService(executable_path=driver_path)
    
    options = FirefoxOptions()
    options.add_argument('--width=250')
    options.add_argument('--height=350')

    driver = webdriver.Firefox(service=service, options=options)
    
    try:
        driver.get(url)
        
        message_input = driver.find_element(By.NAME, "msg")
        captcha_img = driver.find_element(By.CSS_SELECTOR, "img[src*='/feedback/']")
        captcha_input = driver.find_element(By.NAME, "check")
        submit_button = driver.find_element(By.CSS_SELECTOR, "input[type='submit']")

        message_input.send_keys(message)

        while True:
            captcha_value = captcha_input.get_attribute("value")
            if captcha_value:
                break
            time.sleep(1)

        submit_button.click()

        time.sleep(3)

        if "Отправлено." in driver.page_source:
            identifier = driver.find_element(By.CSS_SELECTOR, "strong").text
            print(f" Сообщение отправлено успешно. Ваш идентификатор: {identifier}")
            return True
        else:
            print(" Не удалось найти подтверждение успешной отправки. Проверьте введенные данные.")
            return False

    finally:
        driver.quit()

def main():
    url = "https://anon.fm/feedback"
    message = ""

    success = send_message_with_captcha(url, message)
    if not success:
        print(" Не удалось отправить сообщение.")

if __name__ == "__main__":
    main()
