Rails.application.routes.draw do
  # devise path ways for user model
  devise_for :users, controllers: {
    sessions: "users/sessions",
    registrations: "users/registrations"
  }

  # path to unauthenticated landing page
  # root "users#wakeup"
  get "wakeup", to: "users#wakeup", as: :wakeup

  # authenticated user home page, specific to logged in user
  get "home", to: "users#home", as: :user_home
  get "profile", to: "users#profile", as: :user_profile
  root "users#home"

  resources :users, only: [ :index ]

  resources :trips do
    resources :expenses, only: [ :new, :create, :edit, :update, :destroy ]
    member do
      get "debts"
      post "add_participant"
      delete "leave_trip", action: :leave_trip
      delete "remove_participant/:user_id", action: :remove_participant, as: :remove_participant
    end
  end

  resources :user_trips, only: [] do
    member do
      patch :accept_invitation
      delete :decline_invitation
    end
  end
end
