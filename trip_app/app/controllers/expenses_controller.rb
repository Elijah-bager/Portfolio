class ExpensesController < ApplicationController
  before_action :authenticate_user!
  before_action :set_trip
  before_action :verify_trip_participant
  before_action :set_expense, only: [ :edit, :update, :destroy ]

  def new
    @expense = @trip.expenses.build
  end

  def create
    @expense = @trip.expenses.build(expense_params)
    @expense.spender_id = current_user.id
    if @expense.save
      create_debts
      redirect_to @trip, notice: "Expense added."
    else
      render :new
    end
  end

  def edit
  end

  def update
    if @expense.update(expense_params)
      update_debts
      redirect_to @trip, notice: "Expense updated."
    else
      render :edit
    end
  end

  def destroy
    @expense.destroy
    redirect_to @trip, notice: "Expense deleted."
  end

  private

  def set_trip
    @trip = Trip.find(params[:trip_id])
  end

  def set_expense
    @expense = @trip.expenses.find(params[:id])
  end

  def expense_params
    params.require(:expense).permit(:amount, :category)
  end

  def verify_trip_participant
    accepted_participant = @trip.user_trips.find_by(user: current_user, acceptance: true)
    unless @trip.host == current_user || accepted_participant
      redirect_to trips_path, alert: "You must be a trip participant to manage expenses"
    end
  end

  def create_debts
    return unless params[:expense][:debtor_ids].present?

    debtor_ids = params[:expense][:debtor_ids].reject(&:blank?)
    # Total people sharing: selected debtors + the spender
    total_people = debtor_ids.size + 1
    amount_per_person = @expense.amount / total_people

    debtor_ids.each do |debtor_id|
      @expense.debts.create!(debtor_id: debtor_id, amount: amount_per_person)
    end
  end

  def update_debts
    return unless params[:expense][:debtor_ids].present?

    # Remove old debts
    @expense.debts.destroy_all

    # Create new debts
    debtor_ids = params[:expense][:debtor_ids].reject(&:blank?)
    # Total people sharing: selected debtors + the spender
    total_people = debtor_ids.size + 1
    amount_per_person = @expense.amount / total_people

    debtor_ids.each do |debtor_id|
      @expense.debts.create!(debtor_id: debtor_id, amount: amount_per_person)
    end
  end
end
