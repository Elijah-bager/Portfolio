# frozen_string_literal: true

class Users::RegistrationsController < Devise::RegistrationsController
  before_action :configure_sign_up_params, only: [ :create ]
  before_action :configure_account_update_params, only: [ :update ]
  before_action :ensure_own_account, only: [ :edit, :update, :destroy ]

  # GET /resource/edit
  def edit
    super
  end

  # PUT /resource
  def update
    super
  end

  # DELETE /resource
  def destroy
    # Check if user has any outstanding debts
    outstanding_debts = Debt.joins(:expense).where(debtor_id: resource.id).where("debts.amount > 0")

    if outstanding_debts.any?
      redirect_to edit_user_registration_path, alert: "You cannot delete your account while you have outstanding debts in trips"
      return
    end

    # Check if user is hosting any trips
    if resource.trips.any?
      redirect_to edit_user_registration_path, alert: "You cannot delete your account while hosting trips. Please delete or transfer your trips first."
      return
    end

    super
  end

  protected

  # Ensure user can only edit/delete their own account
  def ensure_own_account
    unless resource == current_user
      redirect_to root_path, alert: "You cannot modify another user's account"
    end
  end

  # If you have extra params to permit, append them to the sanitizer.
  def configure_sign_up_params
    devise_parameter_sanitizer.permit(:sign_up, keys: [ :name ])
  end

  # If you have extra params to permit, append them to the sanitizer.
  def configure_account_update_params
    devise_parameter_sanitizer.permit(:account_update, keys: [ :name ])
  end

  # The path used after sign up.
  def after_sign_up_path_for(resource)
    user_home_path
  end

  # The path used after account update.
  def after_update_path_for(resource)
    user_profile_path
  end
end
