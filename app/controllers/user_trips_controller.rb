class UserTripsController < ApplicationController
  before_action :authenticate_user!

  def accept_invitation
    @user_trip = UserTrip.find(params[:id])
    if @user_trip.user == current_user
      @user_trip.update(acceptance: true)
      redirect_to @user_trip.trip, notice: "You joined the trip!"
    else
      redirect_to root_path, alert: "Unauthorized"
    end
  end

  def decline_invitation
    @user_trip = UserTrip.find(params[:id])
    if @user_trip.user == current_user && !@user_trip.acceptance
      @user_trip.destroy
      redirect_to user_home_path, notice: "Invitation declined"
    else
      redirect_to root_path, alert: "Unauthorized"
    end
  end
end
