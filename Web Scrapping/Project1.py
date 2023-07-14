import tkinter as tk
import tweepy

# Set your Twitter API credentials
consumer_key = 'ml54jM3JjAWNYlOVHr4Q5oiOG'
consumer_secret = 'dAlGZYxafthwy5A9zCaGmBkCPnVeKhA7bt7pO0lyqOgRyKgRJD'
access_token = '1301574746430078976-b8QBgA81QKDx3mhThczQCe2iIFx7ks'
access_token_secret = 'CQegmNDcZAvtozarEta7NOs61nAXR7iexO14VQJtXmuas'

def scrape_twitter_data(username):
    # Authenticate with Twitter API
    auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
    auth.set_access_token(access_token, access_token_secret)
    api = tweepy.API(auth)

    # Scrape tweets from the user's timeline
    tweets = api.user_timeline(screen_name=username, count=10)
    
    tweet_data = []
    for tweet in tweets:
        tweet_text = tweet.text
        tweet_date = tweet.created_at
        tweet_data.append((tweet_text, tweet_date))
    
    return tweet_data

def display_tweets():
    username = entry.get()
    tweet_data = scrape_twitter_data(username)

    for tweet in tweet_data:
        text = tweet[0]
        date = tweet[1]
        text_label = tk.Label(window, text=text)
        text_label.pack()
        date_label = tk.Label(window, text=date)
        date_label.pack()

# Create a GUI window
window = tk.Tk()
window.title("Twitter Scraper")

# Create a label and entry for username input
label = tk.Label(window, text="Enter Twitter Username:")
label.pack()
entry = tk.Entry(window)
entry.pack()

# Create a button to initiate the scraping and display the tweets
button = tk.Button(window, text="Scrape Tweets", command=display_tweets)
button.pack()

# Run the GUI event loop
window.mainloop()
