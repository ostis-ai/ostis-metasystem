# Common configs for all agents
AVAILABLE_LANGUAGES = ('lang_ru', 'lang_en')

# Prompts
PROMPTS = {'lang_en': 'I have sentences divided by semicolons. Connect those sentences in one human-readable text.'
                      'Connect some sentences in one if possible. Drop repeatable information.'
                      'Do not add information which is not represented in sentences. Give only answer, no additional words. Here are sentences: "{!s}"',
           'lang_ru': 'У меня есть предложения, описывающие некоторую сущность, и разделённые точкой с запятой.' 
                      'Соедини эти предложения в читаемый текст на русском языке. Переведи текст с английского на русский с сохранением смысла.'
                      'Объедини некоторые предложения в одно, если возможно. Убери повторяющуюся информацию.'
                      'Не добавляй информацию, которой нет в предложениях.'
                      'Верни только и только читаемый текст, ни слова больше. Вот предложения: "{!s}"'
           }

# Configs for non official API agent
NON_OFFICIAL_API_DEFAULT_MODEL = 'gpt-3.5-turbo'
ACTION_GET_CLEAN_TEXT = 'action_get_clean_text'

