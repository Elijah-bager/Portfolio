class CreateTrips < ActiveRecord::Migration[7.2]
  def change
    create_table :trips do |t|
      t.string :name
      t.references :host, null: false, foreign_key: { to_table: :users }
      # t.integer :id
      t.timestamps
    end
  end
end
