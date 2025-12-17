class AddAcceptanceToUserTrips < ActiveRecord::Migration[7.2]
  def change
    add_column :user_trips, :acceptance, :boolean, default: false, null: false
    add_index :user_trips, :acceptance
  end
end
