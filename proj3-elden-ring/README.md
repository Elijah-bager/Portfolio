# Project
CARMEN_REST_API
## 🧩 Overview
**Carmen Info Dashboard** is a Ruby web application that uses the **Carmen (Canvas) REST API** to retrieve and display your course data, assignments, announcements, and current weather conditions — all in a single, easy-to-read HTML dashboard and as an email sent to you.

The project demonstrates how to connect Ruby (for backend data fetching and logic) with ERB (embedded Ruby in HTML) and CSS for a styled frontend.

---

## ⚙️ Features
- 🔑 **Secure Canvas API integration** using access tokens stored in `.env`
- 📚 Displays a list of your enrolled **courses and assignments**
- 📢 Shows **course announcements**
- 🌤️ Integrates live **weather information** for your city
- 🎨 Clean, styled interface using `styles.css`
- 🧠 Includes dynamic logic and conditional messages (e.g., weather-based tips)
- 🧱 Built with **Sinatra**, **ERB templates**, and **HTTPX** for API requests

---

## 🧰 Tech Stack
| Layer | Technology |
|-------|-------------|
| Language | Ruby |
| View Engine | ERB (Embedded Ruby) |
| Styling | CSS |
| API | Carmen Canvas REST API |
| HTTP Client | HTTPX |
| Environment Management | Dotenv |

---

## 🔐 Setup & Installation


### 2️⃣ Install required gems
```bash
bundle install
```
Make sure your `Gemfile` includes:
```ruby
gem 'httpx'
gem 'dotenv'
gem 'json'
gem 'erb'
```

### 3️⃣ Create a `.env` file
In the project root, create a file named `.env` and add your Carmen (Canvas) access token, Weatherstack api token, and your email:
```bash
CANVAS_TOKEN=your_canvas_access_token_here
WEATHERSTACK_API_KEY=your_api_key_here
USER_EMAIL=your_email_here
```
⚠️ **Important:** Never share or commit this token. Treat it like a password!

---

## 🌐 Running the App

### Start the project:
```bash
ruby lib/main.rb
```

## 🧾 File Structure
```
project_root/
├── lib/
│   └── main.rb           # Main Ruby script that handles API calls and routes
        index.erb         # HTML + ERB template for rendering dynamic content
│   └── styles.css        # CSS stylesheet for page styling
├── .env                  # Stores your API token (ignored by git)
├── .gitignore            # Includes .env and other non-tracked files
└── README.md             # Project documentation
```

---