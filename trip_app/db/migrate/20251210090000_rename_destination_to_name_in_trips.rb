class RenameDestinationToNameInTrips < ActiveRecord::Migration[7.2]
  def change
    # Only run if destination exists and name does not, to be safe in different environments
    if column_exists?(:trips, :destination) && !column_exists?(:trips, :name)
      rename_column :trips, :destination, :name
    end
  end
end
