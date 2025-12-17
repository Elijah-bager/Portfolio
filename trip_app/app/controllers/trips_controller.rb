class TripsController < ApplicationController
  before_action :authenticate_user!
  before_action :set_trip, only: [ :show, :edit, :update, :destroy, :debts, :add_participant, :remove_participant, :leave_trip ]
  before_action :authorize_trip_member, only: [ :show, :edit, :update, :debts ]
  before_action :authorize_trip_host, only: [ :destroy ]
  before_action :authorize_trip_host_or_participant, only: [ :edit, :update ]
  def index
    @trips = Trip.all
    # change to show only user's trips
    # @trips = (current_user.part_trips + current_user.trips).uniq
  end
  def show
    # @trip is loaded by `set_trip` before_action

    # Only host or ACCEPTED participants can view
    accepted_participant = @trip.user_trips.find_by(user: current_user, acceptance: true)

    unless @trip.host == current_user || accepted_participant
      redirect_to trips_path, alert: "You are not authorized to view this trip."
    end
  end

  def debts
    @trip = Trip.find(params[:id])

    # Ensure user is a participant
    unless @trip.host == current_user || @trip.users.include?(current_user)
      redirect_to trips_path, alert: "You are not authorized to view this trip's debts."
      return
    end

    # Calculate balances: positive means they are owed money, negative means they owe
    @balances = {}

    @trip.users.each do |user|
      @balances[user] = 0
    end

    # Process each expense
    @trip.expenses.each do |expense|
      # Calculate how much each person owes for this expense
      debtor_count = expense.debts.count
      # Total people sharing = debtors + spender
      total_people = debtor_count + 1
      amount_per_person = expense.amount / total_people

      # The spender paid the full amount but owes their share
      # Net: they are owed (amount - their_share)
      if @balances.key?(expense.spender)
        @balances[expense.spender] += expense.amount - amount_per_person
      end

      # Each debtor owes their share
      expense.debts.each do |debt|
        debtor = User.find(debt.debtor_id)
        @balances[debtor] -= debt.amount if @balances.key?(debtor)
      end
    end

    # Separate creditors (owed money) and debtors (owe money)
    @creditors = @balances.select { |user, balance| balance > 0.01 }.sort_by { |user, balance| -balance }
    @debtors = @balances.select { |user, balance| balance < -0.01 }.sort_by { |user, balance| balance }
  end

  def new
    @trip = Trip.new
  end

  def create
    @trip = Trip.new(trip_params)
    # associate the current_user as the host
    @trip.host = current_user
    if @trip.save
      # also add the host as a participant
      @trip.user_trips.find_or_create_by(user: current_user, acceptance: true)
      redirect_to @trip
    else
      render :new
    end
  end

  def edit
    @trip = Trip.find(params[:id])
    authorize_trip_host(@trip)
  end

  def update
    @trip = Trip.find(params[:id])
    authorize_trip_host(@trip)

    if @trip.update(trip_params)
      redirect_to @trip, notice: "Trip updated successfully"
    else
      render :edit
    end
  end

  def add_participant
    @trip = Trip.find(params[:id])
    authorize_trip_host_or_participant

    user = User.find_by(id: params[:user_id])
    if user && !@trip.users.include?(user)
      # Send invitation - user must accept before accessing the trip
      @trip.user_trips.create(user: user, acceptance: false)
      redirect_to @trip, notice: "Invitation sent to #{user.name}"
    else
      redirect_to @trip, alert: "User not found or already invited"
    end
  end

  def destroy
    @trip = Trip.find(params[:id])
    @trip.destroy
    flash[:notice] = "Trip deleted successfully."
    redirect_to trips_path
  end

  def remove_participant
    @trip = Trip.find(params[:id])
    authorize_trip_host(@trip)

    user_trip = @trip.user_trips.find_by(user_id: params[:user_id])

    if user_trip
      # Check if user has outstanding debts in this trip
      user_debts = Debt.joins(:expense).where(expenses: { trip_id: @trip.id }, debtor_id: params[:user_id]).where("debts.amount > 0")

      if user_debts.any?
        redirect_to @trip, alert: "Cannot remove participant with outstanding debts"
      else
        user_trip.destroy
        redirect_to @trip, notice: "Participant removed"
      end
    else
      redirect_to @trip, alert: "User not found in trip"
    end
  end

  def leave_trip
    @trip = Trip.find(params[:id])
    user_trip = @trip.user_trips.find_by(user: current_user)

    unless user_trip
      redirect_to trips_path, alert: "You are not a participant in this trip"
      return
    end

    if @trip.host == current_user
      redirect_to @trip, alert: "Trip host cannot leave. Please delete the trip instead."
      return
    end

    # Check if user has outstanding debts
    user_debts = Debt.joins(:expense).where(expenses: { trip_id: @trip.id }, debtor_id: current_user.id).where("debts.amount > 0")

    if user_debts.any?
      redirect_to @trip, alert: "You must settle your debts before leaving this trip" and return
    else
      user_trip.destroy
      redirect_to trips_path, notice: "You have left the trip" and return
    end
  end

  private

  def set_trip
    @trip = Trip.find(params[:id])
  end

  def authorize_trip_member
    # Allow the host or any participant (via user_trips) to access the trip
    unless @trip.host == current_user || @trip.users.exists?(id: current_user.id)
      redirect_to trips_path, alert: "You are not authorized to view this trip." and return
    end
  end

  def trip_params
    # host_id is set server-side to current_user; don't permit from the form
    params.require(:trip).permit(:name, :start_date, :end_date)
  end

  def authorize_trip_host_or_participant
    trip = @trip
    unless trip && (trip.host == current_user || trip.users.exists?(id: current_user.id))
      redirect_to trips_path, alert: "Only the host or participants can edit this trip." and return
    end
  end

  # Ensure a host-only action. Accepts an optional Trip argument for
  # backwards compatibility with calls that pass `@trip` explicitly.
  def authorize_trip_host(trip = nil)
    trip ||= @trip
    unless trip && trip.host == current_user
      redirect_to trips_path, alert: "Only the host can perform that action." and return
    end
  end
end
