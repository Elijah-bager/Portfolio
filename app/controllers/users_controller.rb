class UsersController < ApplicationController
  # users#wakeup is current home / landing page for unauthenticated users
  before_action :authenticate_user!, except: [ :wakeup ]
  before_action :redirect_to_wakeup, only: [ :home, :profile ]
  before_action :set_current_user, only: [ :home, :profile ]

  def wakeup
  end

  def index
    @users = User.all
  end

  # Rest of UsersController actions should be protected by authentication

  def profile
    @user = current_user
  end

  # Home page for authenticated users, should redirect after sign in
  # GET /users/home + user specific parameters
  def home
    # Example: load user-specific data
    # Trips the user is participating in and hosting
    @part_trips = current_user.part_trips
    @hosted_trips = current_user.trips
    # Union of both hosted and participated trips
    @trips = (@part_trips + @hosted_trips).uniq

    # Get pending invitations (not yet accepted)
    @pending_invitations = current_user.user_trips.where(acceptance: false)

    @expenses = current_user.expenses
    @debts = current_user.debts
    # Add more logic as needed
  end

  private

  # Set current user for views, basic authorization gateway
  def set_current_user
    @user = current_user
  end

  def redirect_to_wakeup
    redirect_to wakeup_path unless user_signed_in?
  end
end
