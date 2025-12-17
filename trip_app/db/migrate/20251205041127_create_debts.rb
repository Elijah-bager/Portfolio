class CreateDebts < ActiveRecord::Migration[7.2]
  def change
    create_table :debts do |t|
      t.references :expense, null: false, foreign_key: true
      t.references :debtor, null: false, foreign_key: { to_table: :users }
      t.decimal :amount, precision: 10, scale: 2, null: false
      t.boolean :paid, default: false
      t.timestamps
    end
  end
end
