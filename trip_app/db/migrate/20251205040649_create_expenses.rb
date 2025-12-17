class CreateExpenses < ActiveRecord::Migration[7.2]
  def change
    create_table :expenses do |t|
      t.references :trip, null: false, foreign_key: true
      t.references :spender, null: false, foreign_key: { to_table: :users }
      t.string :category   # your "cut" field
      t.decimal :amount, precision: 10, scale: 2, null: false
      t.timestamps
    end
  end
end
