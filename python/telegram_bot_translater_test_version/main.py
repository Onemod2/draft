import telebot
import re
import requests
import os

from sys import path
path.insert(0, "./test/Include")

# import functions
import configparser

config = configparser.ConfigParser()
config.read('config.ini')
TELEBOT_ID=config['DEFAULT']['TELEBOT_ID']
TRANSLATE_KEY=config['DEFAULT']['TRANSLATE_KEY']

bot = telebot.TeleBot(TELEBOT_ID)

params = {
    "key": TRANSLATE_KEY,
    "text": "Hello",
    "lang": "en-ru"
}

online_users = []
num_of_users = -1

with open(r"./test/main_setting.txt", "w+") as file:  # Count of users
    if os.stat(r"./test/main_setting.txt").st_size == 0:
        file.write("0")
        num_of_users = 0
    else:
        num_of_users = int(re.findall(r"[0-9]+", next(file))[0])

if not os.path.exists("./test/id_matching.txt"):
    with open("./test/id_matching.txt", "w"):
        pass

if not os.path.exists(r"./test/users_data.txt"):
    with open(r"./test/users_data.txt", "w"):
        pass

if not os.path.exists(r"./logs.txt"):
    with open(r"./logs.txt", "w"):
        pass

"""Decorator for logs
"""


def logs_handler(current_func):
    def new_func(message):
        with open(r"./logs.txt", "a") as log_file:
            log_file.write(message.text + "\n")
        current_func(message)
    return new_func


@bot.message_handler(commands=['start'])  # Users start information
@logs_handler
def send_welcome(message):
    bot.send_message(message.chat.id, "Hi, you can use commands:\n"
                                      "/log - for login in account\n"
                                      "/reg - for registration account\n"
                                      "/get - get your translation in txt\n"
                                      "/help - for see an example of use commands\n"
                                      "/about - information about bot\n")


@bot.message_handler(commands=['log'])  # Users login
@logs_handler
def user_login(message):
    data = re.findall(r"(/log ([^ ]*) ([^\\0]*))", message.text)
    if len(data) == 0:
        bot.send_message(message.chat.id, "Incorrect input see /help and try again\n")
    else:
        login = data[0][1]
        password = data[0][2]

        if os.stat(r"./test/users_data.txt").st_size == 0:
            bot.send_message(message.chat.id, "Initially you must create an account\n"
                                              "See command /reg\n")
        else:
            with open(r"./test/users_data.txt", "r") as file:
                for current_string in file:
                    if current_string.split()[0] == login:
                        if current_string.split()[1] == password:
                            bot.send_message(message.chat.id, "Successful")
                            current_user_id = re.findall(r"'from_user': {'id': ([0-9]+)", str(message))[0]

                            # functions.without_string_in_file(current_user_id, r"./test/id_matching.txt")

                            # functions.without_string_in_file(current_string.split()[2], r"./test/id_matching.txt")

                            with open(r"./test/id_matching.txt", "a") as id_file:
                                id_file.write(current_string.split()[2] + ' ' + current_user_id + '\n')
                            break
                        bot.send_message(message.chat.id, "Wrong password")
                        break
                else:
                    bot.send_message(message.chat.id, "Wrong login")


@bot.message_handler(commands=['reg'])  # Users registration
@logs_handler
def user_registration(message):
    global num_of_users
    num_of_users += 1

    data = re.findall(r"(/reg ([^ ]*) ([^\\0]*))", message.text)

    if len(data) == 0:
        bot.send_message(message.chat.id, "Incorrect input see /help and try again\n")
    else:
        login = data[0][1]
        password = data[0][2]
        current_id = str(num_of_users)

        login_is_available = True
        with open(r"./test/users_data.txt", "w+") as file:
            for current_string in file:
                if login == current_string.split()[0]:
                    bot.send_message(message.chat.id, "This login is busy, try again")
                    login_is_available = False

        if login_is_available:
            with open(r"./test/main_setting.txt", "w+") as file:
                file.write(str(num_of_users))

            with open(r"./test/users_data.txt", "a+") as file:
                file.write(login + " " + password + " " + current_id + "\n")

            with open(r"./test/dictionaries/" + str(num_of_users) + ".txt", "w"):
                pass

            bot.send_message(message.chat.id, "Successful")


@bot.message_handler(commands=['get'])  # Get your dictionary
@logs_handler
def get_dict(message):
    current_user_id = re.findall(r"'from_user': {'id': ([0-9]+)", str(message))[0]
    with open("./test/id_matching.txt", "r") as id_file:
        for current_matching in id_file:
            if current_matching.split()[1] == current_user_id:
                current_dictionary = open(r"./test/dictionaries/" + current_matching.split()[0] + ".txt", "rb")
                try:
                    bot.send_document(message.chat.id, current_dictionary)
                except telebot.apihelper.ApiException as e:
                    bot.send_message(message.chat.id, "Can't download it, \n"
                                                      "Maybe your file is empty\n")
                break
        else:
            bot.send_message(message.chat.id, "You must login\n")


@bot.message_handler(commands=['help'])  # Help command for users
@logs_handler
def help_message(message):
    bot.send_message(message.chat.id, "Some examples of commands\n"
                                      "/log test 123\n"
                                      "/reg test 123\n"
                                      "Other commands use without arguments\n")


@bot.message_handler(commands=['about'])  # Information about this bot
@logs_handler
def info_message(message):
    bot.send_message(message.chat.id, "Hello User!\n"
                                      "\n"
                                      "In this program you can translate the words from Eng to Rus and reverse\n"
                                      "But initially you must login and then i can write all your words in\n"
                                      "my dictionary\n"
                                      "In any time you can download your dictionary with words\n"
                                      "Use /get for download it :) "
                                      "\n"
                                      "Thanks for use this bot\n"
                                      "I have been active since 19.05.2020")


@bot.message_handler(func=lambda message: True)  # Handler of usual text message
@logs_handler
def echo_all(message):
    current_user_id = re.findall(r"'from_user': {'id': ([0-9]+)", str(message))[0]
    with open("./test/id_matching.txt", "r") as id_file:
        for current_matching in id_file:
            if current_matching.split()[1] == current_user_id:
                word = message.text
                if re.search(r"[А-Яа-я]", word):
                    params["lang"] = "ru-en"
                else:
                    params["lang"] = "en-ru"

                if word == "":
                    continue

                params["text"] = word.rstrip()
                translation_url_yandex = requests.get("https://translate.yandex.net/api/v1.5/tr.json/translate",
                                                      params=params)
                text = translation_url_yandex.json()
                with open(r"./test/dictionaries/" + current_matching.split()[0] + ".txt", "r") as read_file:
                    for line in read_file:
                        if word in line:
                            bot.send_message(message.chat.id, text["text"][0] +
                                             "\nThis word already exist in your dictionary")
                            break
                    else:
                        if text["text"][0] != word.rstrip():
                            with open(r"./test/dictionaries/" + current_matching.split()[0] + ".txt", "a") as write_file:
                                if params["lang"] == "en-ru":
                                    new_word = "\n" + str(word.rstrip()) + " - {}".format(text["text"][0])
                                else:
                                    new_word = "\n" + "{}".format(text["text"][0]) + " - " + str(word.rstrip())
                                write_file.write(new_word)
                            bot.send_message(message.chat.id, "\"" +
                                             text["text"][0] + "\"" +
                                             "\nIs write to dictionary")
                        else:
                            bot.send_message(message.chat.id, "Don't know this word, try yet")
                break
        else:
            bot.send_message(message.chat.id, r"Initially u must log, use /help")


@bot.message_handler(content_types=['sticker'])  # Handler of stickers
@logs_handler
def return_id(message):
    pattern = r"('sticker': {'file_id': '([^']*))"
    sticker_id = re.findall(pattern, str(message))[0][1]
    bot.send_sticker(message.chat.id, sticker_id)


if __name__ == "__main__":
    bot.polling()
